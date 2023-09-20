

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

void UUnitCondition_Poisoned::OnConditionPersist()
{
	AffectedUnit->ReceiveDamage(iPotency);
	Super::OnConditionPersist();
}

void UUnitCondition_Poisoned::OnConditionRemoved()
{
	Super::OnConditionRemoved();
}
