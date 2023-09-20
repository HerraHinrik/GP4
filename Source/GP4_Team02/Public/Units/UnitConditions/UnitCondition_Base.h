
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UnitCondition_Base.generated.h"

class AUnitBase;
/**
 * 
 */
UCLASS()
class GP4_TEAM02_API UUnitCondition_Base : public UObject
{
	GENERATED_BODY()

protected:
	TObjectPtr<AUnitBase> AffectedUnit;

	int iDuration;
	int iPotency;

public:
	void virtual OnConditionApplied(TObjectPtr<AUnitBase> unit, int duration, int potency);

	bool virtual OnConditionPersist();

	void virtual OnConditionRemoved();
};
