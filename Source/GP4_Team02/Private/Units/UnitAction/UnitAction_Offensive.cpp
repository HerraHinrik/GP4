

#include "Units/UnitAction/UnitAction_Offensive.h"

#include "Units/UnitBase.h"
#include "Units/Unit_Neutral.h"

void UUnitAction_Offensive::StartAction(UTileBase* tile, AUnitBase* unit)
{
	Super::StartAction(tile, unit);
	unit->UsedAttack();
	
	if (tile->GetOccupyingUnit())
	{
		targetEnemy = tile->GetOccupyingUnit();
	}
}

void UUnitAction_Offensive::EndAction()
{
	if (targetEnemy && Action_Unit)
	{
		if (TObjectPtr<AUnit_Neutral> AI_Unit = Cast<AUnit_Neutral>(targetEnemy))
		{
			AI_Unit->WasAttacked(Action_Unit);
		}
	}
	
	Super::EndAction();
	
	
	targetEnemy = nullptr;
}
