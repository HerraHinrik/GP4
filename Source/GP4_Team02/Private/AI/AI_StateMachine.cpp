

#include "AI/AI_StateMachine.h"

#include "AI/AI_StateBase.h"

// stores a state to be added to the stack next tick
void UAI_StateMachine::PushNewState(UAI_StateBase* state)
{
	if (!state)
		return;

	// if (!StateStack.IsEmpty())
	// 	StateStack.Last()->OnStateExit();

	state->Machine = this;
	state->AI_Unit = this->AI_Unit;
	NextState = state;
	
	// state->OnStateBegin();
}

// removes the top state and runs it's OnExit function
void UAI_StateMachine::PopTopState()
{
	if (StateStack.IsEmpty())
		return;
	
	TObjectPtr<UAI_StateBase> topState = StateStack.Pop();
	if (topState)
		topState->OnStateExit();

	//pool removed state?

	
	// if (!StateStack.IsEmpty())
	// 	StateStack.Last()->OnStateBegin();
}
