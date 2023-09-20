#pragma once

#include "CoreMinimal.h"
#include "Units/UnitAction.h"
#include "UnitAction_Supportive.generated.h"

/**
 * 
 */
UCLASS()
class GP4_TEAM02_API UUnitAction_Supportive : public UUnitAction
{
	GENERATED_BODY()

protected:
    TObjectPtr<AUnitBase> targetAlly;

	virtual void EndAction() override;
};
