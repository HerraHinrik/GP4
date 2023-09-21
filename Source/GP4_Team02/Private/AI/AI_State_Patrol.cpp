

#include "AI/AI_State_Patrol.h"

#include "AI/AI_StateMachine.h"
#include "AI/AI_State_ChasePlayerUnit.h"
#include "AI/AI_State_ReturnToPath.h"
#include "GameplaySystems/Team.h"
#include "GameplaySystems/Team_PlayerControlled.h"
#include "Units/Unit_Neutral.h"

TObjectPtr<UTileBase> UAI_State_Patrol::GetNextTile()
{
	//return if no tiles in array
	if (AI_Unit->GetPatrolArea().IsEmpty())
		return nullptr;

	//return if we haven't reached previous "next tile"
	if (NextTile != AI_Unit->GetCurrentTile())
		return NextTile;

	//if we are going back and forth rather than in a circle
	if (!AI_Unit->bCircularPatrol)
	{
		if (bIncrementIndex)
			AI_Unit->iPatrolIndex++;
		else
			AI_Unit->iPatrolIndex--;

		if (AI_Unit->iPatrolIndex >= AI_Unit->GetPatrolArea().Num() - 1 || AI_Unit->iPatrolIndex <= 0)
		{
			bIncrementIndex = !bIncrementIndex;
		}
	}
	else
	{
		AI_Unit->iPatrolIndex++;
		if (AI_Unit->iPatrolIndex >= AI_Unit->GetPatrolArea().Num())
			AI_Unit->iPatrolIndex = 0;
	}

	if (TObjectPtr<UTileBase> outTile = AI_Unit->GetPatrolArea()[AI_Unit->iPatrolIndex])
		return outTile;

	return nullptr;
}

bool UAI_State_Patrol::CheckForEnemies()
{

	if (!AI_Unit)
		return false;

	//get all neighbouring tiles
	TArray<TObjectPtr<UTileBase>> neighbours = AI_Unit->GetAdjacentTiles();
	if (neighbours.IsEmpty())
		return false;

	//if one of the neighbouring tiles have a player-unit set it as the target
	for (TObjectPtr<UTileBase> tile : neighbours)
	{
		if (const TObjectPtr<AUnitBase> Unit = tile->GetOccupyingUnit(); Unit)
		{
			if(const TObjectPtr<ATeam_PlayerControlled> PlayerTeam = Cast<ATeam_PlayerControlled>(Unit->GetTeam()); PlayerTeam)
			{
				AI_Unit->SetNewTarget(tile->GetOccupyingUnit());
				return true;
			}
		}
	}

	return false;
}

void UAI_State_Patrol::OnStateBegin()
{
	Super::OnStateBegin();
	if (AI_Unit)
	{
		OnStateRunning();
	}
}

void UAI_State_Patrol::OnStateRunning()
{
	Super::OnStateRunning();

	if (AI_Unit->GetPatrolArea().IsEmpty())
	{
		AI_Unit->bFinishedMyTurn = true;
		return;
	}
	
	//if we are outside the patrol-area, push return state
	if (!AI_Unit->GetPatrolArea().Contains(AI_Unit->GetCurrentTile()))
	{
		TObjectPtr<UAI_State_ReturnToPath> returnState = NewObject<UAI_State_ReturnToPath>();
		Machine->PushNewState(returnState);
		return;
	}
	
	//if we found an enemy, push chase state
	if (CheckForEnemies())
	{
		TObjectPtr<UAI_State_ChasePlayerUnit> chaseState = NewObject<UAI_State_ChasePlayerUnit>();
		Machine->PushNewState(chaseState);
		return;

	}
	
	//keep on patrollin'
	if (GetNextTile())
	{
		AI_Unit->GetMoveAction()->StartAction(GetNextTile(), AI_Unit);
	}
	
}

void UAI_State_Patrol::OnStateExit()
{
	Super::OnStateExit();
}
