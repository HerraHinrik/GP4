#include "GameBoard/HighlightSystem.h"

#include "GameBoard/GameBoard.h"
#include "GameBoard/GameBoardUtils.h"
#include "GameBoard/Link.h"
#include "GameBoard/Tiles/HexTile_Creation.h"
#include "GameBoard/Tiles/TileBase.h"
#include "GameplaySystems/TWS_GameManager.h"
#include "Units/UnitBase.h"

void UHighlightSystem::Init(TObjectPtr<UTWS_GameManager> InGameManager)
{
	if (InGameManager)
	{
		this->GameManager = InGameManager;
		InGameManager->OnTurnChanged.AddDynamic(this, &UHighlightSystem::ResetAllHighLights);
	}
}

void UHighlightSystem::SetSelectedTile(TObjectPtr<UTileBase> Tile)
{
	ResetAllHighLights();	

	// If null, only reset
	if ( !Tile ) 
	{
		SelectedTile = nullptr;
		return;
	}
	
	if(SelectedTile)
	{
		SelectedTile->OnTileSelected.Broadcast(false);
	}
	
	SelectedTile = Tile;
	SelectedTile->OnTileSelected.Broadcast(true);
	
	// If tile has a unit, highlight and cache neighbours
	TObjectPtr<AUnitBase> Unit = SelectedTile->GetOccupyingUnit();
	if(Unit && Unit->CanAffordAction(1) && ( Unit->CanAttack() || Unit->CanSupport()))
	{
		ResetNeighbourHighLight();
		
		TArray<TObjectPtr<UTileBase>> NeighboursInRange;
		GameBoardUtils::FindNodesWithinRadius(SelectedTile, SelectedTile->GetOccupyingUnit()->iAttackRange, NeighboursInRange);
		for (const TObjectPtr<UTileBase> InRangeTile : NeighboursInRange)
		{
			InRangeTile->OnNeighbour.Broadcast(true);
		}
		NeighbourTiles = NeighboursInRange;
	}
}

void UHighlightSystem::SetHoverTile(TObjectPtr<UTileBase> Tile)
{
	
	ResetHoveredHighLight();
	ResetNeighbourHighLight();
	
	// If null, only reset
	if ( !Tile ) return;
	
	// If we have selected tile, highlight path to hovered tile
	if(SelectedTile)
	{
		if(SelectedTile == Tile)
		{
			SetSelectedTile(SelectedTile);
		}
		else
		{
			TArray<TObjectPtr<ULink>> HoverPath = GameBoardUtils::FindPathInHexGrid(SelectedTile, Tile);
			for (const TObjectPtr<ULink> Link : HoverPath)
			{
				Link->GetTarget()->OnTileHovered.Broadcast(true);
				HoveredTiles.Add(Link->GetTarget());
			}
		}
	}
	else // If not, just highlight hovered tile
	{
		Tile->OnTileHovered.Broadcast(true);
		HoveredTiles.Add(Tile);
	}
}

void UHighlightSystem::HighLightCreationTiles(ATeam* Team)
{

	for (const TObjectPtr<UHexTile_Creation> Tile : CreationTiles)
	{
		TObjectPtr<ATeam> TileTeam = Tile->GetOwningTeam();
		if(TileTeam == Team)
		{
			Tile->OnHighLightCreation.Broadcast(true);
		}
		else
		{
			Tile->OnHighLightCreation.Broadcast(false);
		}
	}
}

void UHighlightSystem::ResetAllHighLights()
{
	if(SelectedTile)
	{
		SelectedTile->OnTileSelected.Broadcast(false);
		SelectedTile = nullptr;
	}

	ResetHoveredHighLight();
	ResetNeighbourHighLight();
	
}

void UHighlightSystem::ResetHoveredHighLight()
{
	for (const TObjectPtr<UTileBase> Tile : HoveredTiles)
	{
		Tile->OnTileHovered.Broadcast(false);
	}
	HoveredTiles.Empty();
}

void UHighlightSystem::ResetNeighbourHighLight()
{
	for (const TObjectPtr<UTileBase> Tile : NeighbourTiles)
	{
		Tile->OnNeighbour.Broadcast(false);
	}
	NeighbourTiles.Empty();
}

void UHighlightSystem::ResetCreationTilesHighLight()
{
	for (const TObjectPtr<UHexTile_Creation> Tile : CreationTiles)
	{
		Tile->OnHighLightCreation.Broadcast(false);
	}
}
