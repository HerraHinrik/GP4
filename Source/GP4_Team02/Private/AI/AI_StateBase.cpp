

#include "AI/AI_StateBase.h"

#include "AI/AI_StateMachine.h"
#include "Units/Unit_Neutral.h"


void UAI_StateBase::OnStateBegin()
{
	AI_Unit = Machine->GetControlledUnit();
}

void UAI_StateBase::OnStateRunning()
{
	AI_Unit->iTurnLoops++;
	if (AI_Unit->iTurnLoops > 10)
	{
		AI_Unit->bFinishedMyTurn = true;
	}
}

void UAI_StateBase::OnStateExit()
{
	Machine->GetCurrentState()->OnStateRunning();
}
