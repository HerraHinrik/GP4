

#include "Units/UnitAction/Imp_PoisonAction.h"

#include "GameBoard/GameBoardUtils.h"
#include "Units/UnitBase.h"
#include "Units/Unit_Devil_Imp.h"
#include "Units/UnitConditions/UnitCondition_Poisoned.h"

void UImp_PoisonAction::StartAction(UTileBase* tile, AUnitBase* unit)
{
	//null checks
	if (!tile || !unit)
		return;

	//other checks
	if (!bCanPerformAction || !tile->GetOccupyingUnit() || !unit->CanAttack())
		return;
	

	TObjectPtr<UTileBase> startTile =  unit->GetCurrentTile();
	TObjectPtr<AUnit_Devil_Imp> imp = Cast<AUnit_Devil_Imp>(unit);
	if (!startTile || !imp)
		return;

	//check if target is in range
	TArray<TObjectPtr<UTileBase>> tiles;
	GameBoardUtils::FindNodesWithinRadius(startTile, imp->iAttackRange, tiles);
	if (!tiles.Contains(tile))
		return;
	
	//check if we get the enemy
	targetEnemy = tile->GetOccupyingUnit();
	if (!targetEnemy) { return; }

	//cache values
	Super::StartAction(tile, unit);

	
	//poison the enemy
	TObjectPtr<UUnitCondition_Poisoned> poison = NewObject<UUnitCondition_Poisoned>();
	if (poison)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Poison!!");
		targetEnemy->AddCondition(poison, imp->iPoisonDuration, imp->iAttackDamage);
	}

	ExecuteAction();
}

void UImp_PoisonAction::ExecuteAction()
{
	Super::ExecuteAction();

	EndAction();
}

void UImp_PoisonAction::EndAction()
{
	Super::EndAction();
}
