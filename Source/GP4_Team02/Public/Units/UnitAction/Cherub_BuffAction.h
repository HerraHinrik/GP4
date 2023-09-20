
#pragma once

#include "CoreMinimal.h"
#include "UnitAction_Supportive.h"
#include "Cherub_BuffAction.generated.h"

/**
 * 
 */
UCLASS()
class GP4_TEAM02_API UCherub_BuffAction : public UUnitAction_Supportive
{
	GENERATED_BODY()

public:
	virtual void StartAction(UTileBase* tile, AUnitBase* unit) override;
	
protected:
	virtual void ExecuteAction() override;
	virtual void EndAction() override;
	
};
