

#include "Units/UnitConditions/UnitCondition_Poisoned.h"

#include "Units/UnitBase.h"

UUnitCondition_Poisoned::UUnitCondition_Poisoned()
{
	iDuration = 2;
}

void UUnitCondition_Poisoned::OnConditionApplied(TObjectPtr<AUnitBase> unit, int duration, int potency)
{
	Super::OnConditionApplied(unit, duration, potency);
}

bool UUnitCondition_Poisoned::OnConditionPersist()
{
	if (AffectedUnit)
	{
		OnPoisonTick.Broadcast(AffectedUnit->GetCurrentTile());
		AffectedUnit->ReceiveDamage(iPotency);
	}
	
	return Super::OnConditionPersist();
}

void UUnitCondition_Poisoned::OnConditionRemoved()
{
	Super::OnConditionRemoved();
}
