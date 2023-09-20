
#pragma once

#include "CoreMinimal.h"
#include "Units/UnitConditions/UnitCondition_Base.h"
#include "UnitCondition_AttackBuff.generated.h"


UCLASS()
class GP4_TEAM02_API UUnitCondition_AttackBuff : public UUnitCondition_Base
{
	GENERATED_BODY()

	
public:
	UUnitCondition_AttackBuff();
	
	virtual void OnConditionApplied(TObjectPtr<AUnitBase> unit, int duration, int potency) override;

	virtual void OnConditionPersist() override;

	virtual void OnConditionRemoved() override;
};
