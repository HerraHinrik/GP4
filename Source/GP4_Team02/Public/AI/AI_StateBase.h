
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI_StateBase.generated.h"

class AUnit_Neutral;
class UAI_StateMachine;

UCLASS()
class GP4_TEAM02_API UAI_StateBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UAI_StateMachine> Machine;
	UPROPERTY()
	TObjectPtr<AUnit_Neutral> AI_Unit;
	
	UFUNCTION()
	virtual void OnStateBegin();
	UFUNCTION()
	virtual void OnStateRunning();
	UFUNCTION()
	virtual void OnStateExit();

	UFUNCTION()
	void SetAIUnit(AUnit_Neutral* Unit) { AI_Unit = Unit; }
};
