

#include "Units/UnitAction/Healer_HealAction.h"

#include "GameBoard/GameBoardUtils.h"
#include "Units/UnitBase.h"
#include "Units/Unit_Angel_Healer.h"

void UHealer_HealAction::StartAction(UTileBase* tile, AUnitBase* unit)
{
	//null checks
	if (!tile || !unit)
		return;

	//other checks
	if (!bCanPerformAction || !tile->GetOccupyingUnit())
		return;
	

	TObjectPtr<UTileBase> startTile =  unit->GetCurrentTile();
	TObjectPtr<AUnit_Angel_Healer> healer = Cast<AUnit_Angel_Healer>(unit);
	if (!startTile || !healer)
		return;

	//check if target is in range
	TArray<TObjectPtr<UTileBase>> tiles;
	GameBoardUtils::FindNodesWithinRadius(startTile, healer->iHealActionRange, tiles);
	if (!tiles.Contains(tile))
		return;
	
	//check if we get the ally
	targetAlly = tile->GetOccupyingUnit();
	if (!targetAlly) { return; }

	//cache values
	UUnitAction::StartAction(tile, unit);

	
	//heal the ally
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Healing!!");
	targetAlly->ReceiveHealing(healer->iHealAmount);

	ExecuteAction();
}

void UHealer_HealAction::ExecuteAction()
{
	Super::ExecuteAction();
	
	EndAction();
}

void UHealer_HealAction::EndAction()
{
	Super::EndAction();
}
