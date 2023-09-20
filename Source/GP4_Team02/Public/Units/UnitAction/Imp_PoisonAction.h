
#pragma once

#include "CoreMinimal.h"
#include "UnitAction_Offensive.h"
#include "Imp_PoisonAction.generated.h"

/**
 * 
 */
UCLASS()
class GP4_TEAM02_API UImp_PoisonAction : public UUnitAction_Offensive
{
	GENERATED_BODY()

public:
	virtual void StartAction(UTileBase* tile, AUnitBase* unit) override;
	
protected:
	virtual void ExecuteAction() override;
	virtual void EndAction() override;
};
