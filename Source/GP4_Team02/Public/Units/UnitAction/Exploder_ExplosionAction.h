
#pragma once

#include "CoreMinimal.h"
#include "Units/UnitAction.h"
#include "Exploder_ExplosionAction.generated.h"

/**
 * 
 */
UCLASS()
class GP4_TEAM02_API UExploder_ExplosionAction : public UUnitAction
{
	GENERATED_BODY()

public:
	virtual void StartAction(UTileBase* tile, AUnitBase* unit) override;

protected:
	virtual void ExecuteAction() override;
	virtual void EndAction() override;
	
};
