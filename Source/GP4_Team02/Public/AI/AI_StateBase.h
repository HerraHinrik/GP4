
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

protected:
	TObjectPtr<UAI_StateMachine> Machine;
	TObjectPtr<AUnit_Neutral> AI_Unit;
	
public:
	void virtual OnStateBegin();
	void virtual OnStateRunning();
	void virtual OnStateExit();

	void SetMachine(TObjectPtr<UAI_StateMachine> machine) { Machine = machine; }
	TObjectPtr<UAI_StateMachine> GetMachine() const { return Machine; }
};
