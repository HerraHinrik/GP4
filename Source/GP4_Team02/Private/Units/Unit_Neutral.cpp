

#include "Units/Unit_Neutral.h"

#include "AI/AI_StateBase.h"
#include "AI/AI_StateMachine.h"
#include "AI/AI_State_Patrol.h"
#include "GameBoard/GameBoardUtils.h"


void AUnit_Neutral::BeginPlay()
{
	Super::BeginPlay();

	StateMachine = NewObject<UAI_StateMachine>();
	
	if (StateMachine)
	{
		TObjectPtr<UAI_State_Patrol> patrolState = NewObject<UAI_State_Patrol>();
		StateMachine->GetStateStack().Add(patrolState);
	}

	

	
}

bool AUnit_Neutral::CheckTargetInRange()
{
	//get all nodes within attack range
	TArray<TObjectPtr<UTileBase>> tiles;
	GameBoardUtils::GetNodesWithinRadius(GetCurrentTile()->GetWorldLocation(),tiles, iAttackRange);

	//early out if empty array
	if (tiles.IsEmpty())
		return false;

	//if target is in range of attack return true
	if (tiles.Contains(GetTargetUnit()->GetCurrentTile()))
	{
		return true;
	}

	return false;
}

void AUnit_Neutral::ExecuteCurrentState()
{
	StateMachine->GetCurrentState()->OnStateRunning();

	if (iUnitActionPoints <= 0)
	{
		bFinishedMyTurn = true;
	}
}

void AUnit_Neutral::ResetUnit()
{
	Super::ResetUnit();

	bFinishedMyTurn = false;
}


