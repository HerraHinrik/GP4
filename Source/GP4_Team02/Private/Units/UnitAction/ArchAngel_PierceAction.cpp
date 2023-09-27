

#include "Units/UnitAction/ArchAngel_PierceAction.h"

#include "GameBoard/GameBoard.h"
#include "GameBoard/GameBoardUtils.h"
#include "GameBoard/Tiles/HexTile.h"
#include "Kismet/KismetSystemLibrary.h"
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
	
	//try to find a tile behind first target
	TObjectPtr<UHexTile> HexTile = Cast<UHexTile>(tile);
	TObjectPtr<UHexTile> StartHexTile = Cast<UHexTile>(startTile);
	if(!HexTile || !StartHexTile) return;
	
	FHexCoordinates directionCoords = HexTile->GetHexCoordinates() - StartHexTile->GetHexCoordinates();

	rearTile = GameBoardUtils::FindNodeByHexCoordinates(	HexTile->GetHexCoordinates() + directionCoords, tile->GetGameBoardParent()->NodeTiles);

	//cache values, start action
	UUnitAction::StartAction(tile, unit);
	
	ExecuteAction();
}

void UArchAngel_PierceAction::ExecuteAction()
{
	Super::ExecuteAction();
	//hit the targets
	targetEnemy->ReceiveDamage(Action_Unit->iAttackDamage);
	
	if (rearTile)
	{
		if (TObjectPtr<AUnitBase> secondTarget = rearTile->GetOccupyingUnit())
		{
			secondTarget->ReceiveDamage(Action_Unit->iAttackDamage);
		}
	}
	EndAction();
}

void UArchAngel_PierceAction::EndAction()
{
	rearTile = nullptr;
	Super::EndAction();
}
