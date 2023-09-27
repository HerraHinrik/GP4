

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
	TObjectPtr<AUnit_Devil_ArchDemon> Demon = Cast<AUnit_Devil_ArchDemon>(unit);
	if (!startTile || !Demon)
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
	UUnitAction::StartAction(tile, unit);
	
	//add all overlapping tiles to target array
	TArray<TObjectPtr<UTileBase>> tilesToSwipe;
	tilesToSwipe.Add(tile);
	for (TObjectPtr<UTileBase> neighbourTile : myNeighbours)
	{
		if (targetNeighbours.Contains(neighbourTile) && neighbourTile != tile && neighbourTile != unit->GetCurrentTile())
		{
			tilesToSwipe.Add(neighbourTile);
		}
	}

	for (TObjectPtr<UTileBase> swipeTile : tilesToSwipe)
	{
		if (swipeTile->GetOccupyingUnit())
		{
			swipeTile->GetOccupyingUnit()->ReceiveDamage(Demon->iAttackDamage);
		}
	}

	ExecuteAction();
}

void UArchDemon_SwipeAction::ExecuteAction()
{
	Super::ExecuteAction();
	
	EndAction();
}

void UArchDemon_SwipeAction::EndAction()
{
	Super::EndAction();
}
