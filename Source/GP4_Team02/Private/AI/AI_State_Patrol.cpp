

#include "AI/AI_State_Patrol.h"

#include "AI/AI_StateMachine.h"
#include "AI/AI_State_ChasePlayerUnit.h"
#include "AI/AI_State_ReturnToPath.h"
#include "GameBoard/Tiles/TileBase.h"
#include "GameplaySystems/Team_PlayerControlled.h"
#include "Units/Unit_Neutral.h"

bool UAI_State_Patrol::GotNextTile()
{
	//return if no tiles in array
	if (AI_Unit->GetPatrolArea().IsEmpty())
		return false;

	//return if we haven't reached previous "next tile"
	if (NextTile && NextTile != AI_Unit->GetCurrentTile())
		return true;

	if (AI_Unit->GetPatrolArea().Contains(AI_Unit->GetCurrentTile())
		&& AI_Unit->GetCurrentTile() != AI_Unit->GetPatrolArea()[AI_Unit->iPatrolIndex])
	{
		SetDirection();
	}

	//if we are going back and forth rather than in a circle
	if (!AI_Unit->bCircularPatrol)
	{
		
		if (bIncrementIndex)
		{
			AI_Unit->iPatrolIndex++;
			if (AI_Unit->iPatrolIndex >= AI_Unit->GetPatrolArea().Num())
			{
				AI_Unit->iPatrolIndex = AI_Unit->GetPatrolArea().Num() - 1;
			}
		}
		else
		{
			AI_Unit->iPatrolIndex--;
			if (AI_Unit->iPatrolIndex < 0)
			{
				AI_Unit->iPatrolIndex = 0;
			}
		}
		
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

	if (TObjectPtr<UTileBase> nextTile = AI_Unit->GetPatrolArea()[AI_Unit->iPatrolIndex])
	{
		NextTile = nextTile;
		return true;
	}
	return false;
}

void UAI_State_Patrol::SetDirection()
{
	AI_Unit->SetupIndex();
	
	int index = AI_Unit->iPatrolIndex;
	if (index >= 3)
		bIncrementIndex = false;
}

bool UAI_State_Patrol::CheckForEnemies()
{
	if (!AI_Unit)
		return false;

	//get all neighbouring tiles
	TArray<TObjectPtr<UTileBase>> neighbours;
	AI_Unit->GetAdjacentTiles(neighbours);
	if (neighbours.IsEmpty())
		return false;

	//if one of the neighbouring tiles have a player-unit set it as the target
	for (TObjectPtr<UTileBase> tile : neighbours)
	{
		if (const TObjectPtr<AUnitBase> Unit = tile->GetOccupyingUnit())
		{
			if(const TObjectPtr<ATeam_PlayerControlled> PlayerTeam = Cast<ATeam_PlayerControlled>(Unit->GetTeam()); PlayerTeam
				&& !Unit->IsInSafeZone())
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

	
	if (AI_Unit->GetPatrolArea().IsEmpty() || AI_Unit->GetPatrolArea().Num() <= 0)
	{
		AI_Unit->bFinishedMyTurn = true;
		return;
	}

	if (bFirstTimePatrolling)
	{
		SetDirection();
		bFirstTimePatrolling = false;
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
	if (GotNextTile())
	{
		AI_Unit->GetMoveAction()->StartAction(NextTile, AI_Unit);
	}
	GEngine->AddOnScreenDebugMessage( -1, 10.f, FColor::Blue, "Patrol" );
	AI_Unit->bFinishedMyTurn = true;
}

void UAI_State_Patrol::OnStateExit()
{
	Super::OnStateExit();
}
