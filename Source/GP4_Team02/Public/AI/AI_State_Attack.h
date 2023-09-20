
#pragma once

#include "CoreMinimal.h"
#include "AI/AI_StateBase.h"
#include "AI_State_Attack.generated.h"

/**
 * 
 */
UCLASS()
class GP4_TEAM02_API UAI_State_Attack : public UAI_StateBase
{
	GENERATED_BODY()

public:
	virtual void OnStateBegin() override;
	virtual void OnStateRunning() override;
	virtual void OnStateExit() override;
};
