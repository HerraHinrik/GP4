

#include "Units/UnitAction/ArchDemon_SwipeAction.h"

#include "GameBoard/GameBoardUtils.h"
#include "GameBoard/Tiles/TileBase.h"
#include "Units/UnitBase.h"
#include "Units/Unit_Devil_ArchDemon.h"

void UArchDemon_SwipeAction::StartAction(UTileBase* tile, AUnitBase* unit)
{
	//null checks
	if (!tile || !unit)
		return;

	//other checks
	if (!bCanPerformAction || !tile->GetOccupyingUnit() || !unit->CanAttack())
		return;

	TObjectPtr<UTileBase> startTile =  unit->GetCurrentTile();
	if (!startTile)
		return;

	//check if target is in range
	TArray<TObjectPtr<UTileBase>> tiles;
	GameBoardUtils::FindNodesWithinRadius(startTile, 1, tiles);
	if (!tiles.Contains(tile))
		return;
	
	//get first target
	targetEnemy = tile->GetOccupyingUnit();
	if (!targetEnemy) { return; }

	//get neighbours of unit and target
	TArray<TObjectPtr<UTileBase>> myNeighbours;
	unit->GetAdjacentTiles(myNeighbours);
	TArray<TObjectPtr<UTileBase>> targetNeighbours;
	targetEnemy->GetAdjacentTiles(targetNeighbours);
	if (myNeighbours.IsEmpty() || targetNeighbours.IsEmpty())
		return;

	//cache values, start action
	Super::StartAction(tile, unit);
	
	//add all overlapping tiles to target array
	swipeArea.Add(tile);
	for (TObjectPtr<UTileBase> neighbourTile : myNeighbours)
	{
		if (targetNeighbours.Contains(neighbourTile) && neighbourTile != tile && neighbourTile != unit->GetCurrentTile())
		{
			swipeArea.Add(neighbourTile);
		}
	}

	ExecuteAction();
}

void UArchDemon_SwipeAction::ExecuteAction()
{
	Super::ExecuteAction();

	for (TObjectPtr<UTileBase> swipeTile : swipeArea)
	{
		if (swipeTile->GetOccupyingUnit())
		{
			swipeTile->GetOccupyingUnit()->ReceiveDamage(Action_Unit->iAttackDamage);
		}
	}
	
	EndAction();
}

void UArchDemon_SwipeAction::EndAction()
{
	Super::EndAction();
}
