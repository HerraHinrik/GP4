

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

bool UUnitCondition_Base::OnConditionPersist()
{
	iDuration--;
	if (iDuration <= 0)
	{
		OnConditionRemoved();
		return false;
	}
	return true;
}

void UUnitCondition_Base::OnConditionRemoved()
{
	AffectedUnit = nullptr;
	//destroy self? pool self?
}
