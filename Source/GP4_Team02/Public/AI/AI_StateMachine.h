
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI_StateMachine.generated.h"

class AUnit_Neutral;
class UAI_StateBase;
/**
 * 
 */
UCLASS()
class GP4_TEAM02_API UAI_StateMachine : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<AUnit_Neutral> AI_Unit;
	
public:
	UPROPERTY()
	TArray<TObjectPtr<UAI_StateBase>> StateStack;

	TObjectPtr<UAI_StateBase> GetCurrentState() const
	{
		if (!StateStack.IsEmpty() && StateStack.Num() > 0)
		{
			TObjectPtr<UAI_StateBase> state = StateStack.Last();
			if (state)
			{
				return state;
			}
			
		}
		return nullptr;
	}

	void PushNewState(TObjectPtr<UAI_StateBase> state);

	void PopTopState();

	void SetControlledUnit(TObjectPtr<AUnit_Neutral> unit) { AI_Unit = unit; }
	TObjectPtr<AUnit_Neutral> GetControlledUnit() const { return AI_Unit; }
	// TArray<TObjectPtr<UAI_StateBase>> GetStateStack() const { return StateStack ;}
	
};
