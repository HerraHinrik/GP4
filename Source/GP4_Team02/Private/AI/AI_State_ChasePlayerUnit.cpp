

#include "AI/AI_State_ChasePlayerUnit.h"

#include "AI/AI_StateMachine.h"
#include "AI/AI_State_Attack.h"
#include "GameBoard/GameBoard.h"
#include "GameBoard/GameBoardUtils.h"
#include "GameBoard/Link.h"
#include "Units/Unit_Neutral.h"



void UAI_State_ChasePlayerUnit::OnStateBegin()
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

void UAI_State_ChasePlayerUnit::OnStateRunning()
{
	Super::OnStateRunning();

	//if enemy in range, push attack state
	if (AI_Unit->CheckTargetInRange())
	{
		TObjectPtr<UAI_State_Attack> attackState = NewObject<UAI_State_Attack>();
		Machine->PushNewState(attackState);
	}
	else
	{
		//if target is dead leave this state
		if (AI_Unit->GetTargetUnit() && AI_Unit->GetTargetUnit()->IsUnitAlive())
		{
			TArray<TObjectPtr<ULink>> Path = GameBoardUtils::FindPathInHexGrid(AI_Unit->GetCurrentTile(), AI_Unit->GetTargetUnit()->GetCurrentTile());
			AI_Unit->GetMoveAction()->StartAction(Path[0]->GetTarget(), AI_Unit);
		}
		else
		{
			Machine->PopTopState();
		}
	}
}

void UAI_State_ChasePlayerUnit::OnStateExit()
{
	Super::OnStateExit();
}
