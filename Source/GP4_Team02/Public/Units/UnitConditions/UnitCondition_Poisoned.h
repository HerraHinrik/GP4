
#pragma once

#include "CoreMinimal.h"
#include "Units/UnitConditions/UnitCondition_Base.h"
#include "UnitCondition_Poisoned.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPoisonTick, UTileBase*, tile);

UCLASS()
class GP4_TEAM02_API UUnitCondition_Poisoned : public UUnitCondition_Base
{
	GENERATED_BODY()

public:
	UUnitCondition_Poisoned();
	
	virtual void OnConditionApplied(TObjectPtr<AUnitBase> unit, int duration, int potency) override;

	virtual bool OnConditionPersist() override;

	virtual void OnConditionRemoved() override;

	UPROPERTY(BlueprintAssignable)
	FOnPoisonTick OnPoisonTick;
};
