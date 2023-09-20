
#pragma once

#include "CoreMinimal.h"
#include "UnitAction_Offensive.h"
#include "ArchAngel_PierceAction.generated.h"

/**
 * 
 */
UCLASS()
class GP4_TEAM02_API UArchAngel_PierceAction : public UUnitAction_Offensive
{
	GENERATED_BODY()

public:
	virtual void StartAction(UTileBase* tile, AUnitBase* unit) override;

protected:
	virtual void ExecuteAction() override;
	virtual void EndAction() override;
};
