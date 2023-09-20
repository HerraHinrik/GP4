
#pragma once

#include "CoreMinimal.h"
#include "Units/UnitAction.h"
#include "UnitAction_Offensive.generated.h"

/**
 * 
 */
UCLASS()
class GP4_TEAM02_API UUnitAction_Offensive : public UUnitAction
{
	GENERATED_BODY()

public:
	virtual void StartAction(UTileBase* tile, AUnitBase* unit) override;
	
protected:
	TObjectPtr<AUnitBase> targetEnemy;

	virtual void EndAction() override;
};
