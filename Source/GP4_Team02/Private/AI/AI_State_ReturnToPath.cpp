

#include "AI/AI_State_ReturnToPath.h"

#include "AI/AI_StateMachine.h"
#include "Units/Unit_Neutral.h"

void UAI_State_ReturnToPath::OnStateBegin()
{
	Super::OnStateBegin();
	if (AI_Unit)
	{
		OnStateRunning();
	}
}

void UAI_State_ReturnToPath::OnStateRunning()
{
	Super::OnStateRunning();

	if (AI_Unit->GetPatrolArea().Contains(AI_Unit->GetCurrentTile()))
	{
		AI_Unit->SetupIndex();
		Machine->PopTopState();
	}
	else
	{
		AI_Unit->GetMoveAction()->StartAction(AI_Unit->GetPatrolArea()[AI_Unit->iPatrolIndex], AI_Unit);
	}
}

void UAI_State_ReturnToPath::OnStateExit()
{
	Super::OnStateExit();
}
