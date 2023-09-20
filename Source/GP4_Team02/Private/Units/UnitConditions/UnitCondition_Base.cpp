

#include "Units/UnitConditions/UnitCondition_Base.h"

#include "Units/UnitBase.h"

void UUnitCondition_Base::OnConditionApplied(TObjectPtr<AUnitBase> unit, int duration, int potency)
{
	if (!unit)
		return;
	iDuration = duration;
	iPotency = potency;
	AffectedUnit = unit;
}

void UUnitCondition_Base::OnConditionPersist()
{
	iDuration--;
	if (iDuration <= 0)
		OnConditionRemoved();
}

void UUnitCondition_Base::OnConditionRemoved()
{
	AffectedUnit->RemoveCondition(this);
	AffectedUnit = nullptr;
	//destroy self? pool self?
}
