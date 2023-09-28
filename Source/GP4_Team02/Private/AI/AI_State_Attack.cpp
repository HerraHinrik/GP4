

#include "AI/AI_State_Attack.h"

#include "AI/AI_StateMachine.h"
#include "Units/Unit_Neutral.h"

void UAI_State_Attack::OnStateBegin()
{
	Super::OnStateBegin();
	if (AI_Unit)
	{
		if (AI_Unit->GetTargetUnit() && AI_Unit->GetTargetUnit()->IsUnitAlive())
		{
			OnStateRunning();
		}
		else
		{
			Machine->PopTopState();
		}
	}
}

void UAI_State_Attack::OnStateRunning()
{
	Super::OnStateRunning();

	//if we still have a target and it's in range, attack it
	if (AI_Unit->GetTargetUnit() && AI_Unit->CheckTargetInRange())
	{
		AI_Unit->GetOffensiveAction()->StartAction(AI_Unit->GetTargetUnit()->GetCurrentTile(), AI_Unit);
	}
	//otherwise leave this state
	else
	{
		Machine->PopTopState();
	}
}

void UAI_State_Attack::OnStateExit()
{
	Super::OnStateExit();
}
