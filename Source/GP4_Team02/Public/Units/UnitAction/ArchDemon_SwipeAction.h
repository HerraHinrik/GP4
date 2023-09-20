
#pragma once

#include "CoreMinimal.h"
#include "UnitAction_Offensive.h"
#include "ArchDemon_SwipeAction.generated.h"

/**
 * 
 */
UCLASS()
class GP4_TEAM02_API UArchDemon_SwipeAction : public UUnitAction_Offensive
{
	GENERATED_BODY()

public:
	virtual void StartAction(UTileBase* tile, AUnitBase* unit) override;
	
protected:
	virtual void ExecuteAction() override;
	virtual void EndAction() override;
};
