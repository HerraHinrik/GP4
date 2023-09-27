

#include "Units/UnitAction/Cherub_BuffAction.h"

#include "GameBoard/GameBoardUtils.h"
#include "Units/UnitBase.h"
#include "Units/Unit_Angel_Cherub.h"
#include "Units/UnitConditions/UnitCondition_AttackBuff.h"

void UCherub_BuffAction::StartAction(UTileBase* tile, AUnitBase* unit)
{
	//null checks
	if (!tile || !unit)
		return;

	//other checks
	if (!bCanPerformAction || !tile->GetOccupyingUnit() || !unit->CanSupport())
		return;
	

	TObjectPtr<UTileBase> startTile =  unit->GetCurrentTile();
	TObjectPtr<AUnit_Angel_Cherub> cherub = Cast<AUnit_Angel_Cherub>(unit);
	if (!startTile || !cherub)
		return;

	//check if target is in range
	TArray<TObjectPtr<UTileBase>> tiles;
	GameBoardUtils::FindNodesWithinRadius(startTile, cherub->iBuffActionRange, tiles);
	if (!tiles.Contains(tile))
		return;
	
	//check if we get the ally
	targetAlly = tile->GetOccupyingUnit();
	if (!targetAlly) { return; }
	
	//cache values
	Super::StartAction(tile, unit);

	
	//buff the ally
	TObjectPtr<UUnitCondition_AttackBuff> buff = NewObject<UUnitCondition_AttackBuff>();
	if (buff)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Buffing Up!!");
		targetAlly->AddCondition(buff, cherub->iBuffDuration, cherub->iBuffAmount);
	}
	
	ExecuteAction();
}

void UCherub_BuffAction::ExecuteAction()
{
	Super::ExecuteAction();

	EndAction();
}

void UCherub_BuffAction::EndAction()
{
	Super::EndAction();
}
