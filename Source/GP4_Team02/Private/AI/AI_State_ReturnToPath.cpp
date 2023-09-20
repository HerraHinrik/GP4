

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
		Machine->PopTopState();
	}
	else
	{
		// pathfind back to patrol area
	}
}

void UAI_State_ReturnToPath::OnStateExit()
{
	Super::OnStateExit();
}
