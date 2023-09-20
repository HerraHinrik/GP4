
#pragma once

#include "CoreMinimal.h"
#include "UnitAction_Supportive.h"
#include "Healer_HealAction.generated.h"

/**
 * 
 */
UCLASS()
class GP4_TEAM02_API UHealer_HealAction : public UUnitAction_Supportive
{
	GENERATED_BODY()

public:
	virtual void StartAction(UTileBase* tile, AUnitBase* unit) override;

protected:
	virtual void ExecuteAction() override;
	virtual void EndAction() override;
};
