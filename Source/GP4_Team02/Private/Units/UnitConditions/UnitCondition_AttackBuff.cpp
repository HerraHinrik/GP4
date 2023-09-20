

#include "Units/UnitConditions/UnitCondition_AttackBuff.h"

#include "Units/UnitBase.h"

UUnitCondition_AttackBuff::UUnitCondition_AttackBuff()
{
	iDuration = 1;
}

void UUnitCondition_AttackBuff::OnConditionApplied(TObjectPtr<AUnitBase> unit, int duration, int potency)
{
	Super::OnConditionApplied(unit, duration, potency);
	if (AffectedUnit)
		AffectedUnit->iAttackDamage += iPotency;
}

bool UUnitCondition_AttackBuff::OnConditionPersist()
{
	return Super::OnConditionPersist();
}

void UUnitCondition_AttackBuff::OnConditionRemoved()
{
	if (AffectedUnit)
		AffectedUnit->iAttackDamage -= iPotency;
	
	Super::OnConditionRemoved();
}
