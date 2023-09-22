

#include "Units/UnitAction/ArchAngel_PierceAction.h"

#include "GameBoard/GameBoardUtils.h"
#include "Units/UnitBase.h"
#include "Units/Unit_Angel_ArchAngel.h"

void UArchAngel_PierceAction::StartAction(UTileBase* tile, AUnitBase* unit)
{
	//null checks
	if (!tile || !unit)
		return;

	//other checks
	if (!bCanPerformAction || !tile->GetOccupyingUnit() || !unit->CanAttack())
		return;

	
	TObjectPtr<UTileBase> startTile =  unit->GetCurrentTile();
	TObjectPtr<AUnit_Angel_ArchAngel> Angel = Cast<AUnit_Angel_ArchAngel>(unit);
	if (!startTile || !Angel)
		return;

	//check if target is in range
	TArray<TObjectPtr<UTileBase>> tiles;
	GameBoardUtils::FindNodesWithinRadius(startTile, 1, tiles);
	if (!tiles.Contains(tile))
		return;

	
	//get first target
	targetEnemy = tile->GetOccupyingUnit();
	if (!targetEnemy) { return; }

	//try to find a tile behind first target
	TObjectPtr<UTileBase> rearTile;
	FHexCoordinates directionCoords = tile->GetCoordinates() - startTile->GetCoordinates();
	
	TArray<TObjectPtr<UTileBase>> myNeighbours;
	unit->GetAdjacentTiles(myNeighbours);
	
	for (TObjectPtr<UTileBase> neighbourTile : myNeighbours)
	{
		FHexCoordinates neighbourDirectionCoords = neighbourTile->GetCoordinates() - tile->GetCoordinates();
		if (directionCoords == neighbourDirectionCoords)
		{
			rearTile = neighbourTile;
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Found Pierce Tile");
			break;
		}
	}

	//cache values, start action
	UUnitAction::StartAction(tile, unit);

	//hit the targets
	targetEnemy->ReceiveDamage(Angel->iPierceDamage);
	
	if (rearTile)
	{
		if (TObjectPtr<AUnitBase> secondTarget = rearTile->GetOccupyingUnit())
		{
			secondTarget->ReceiveDamage(Angel->iPierceDamage);
		}
	}

	ExecuteAction();
}

void UArchAngel_PierceAction::ExecuteAction()
{
	Super::ExecuteAction();
	
	EndAction();
}

void UArchAngel_PierceAction::EndAction()
{
	Super::EndAction();
}
