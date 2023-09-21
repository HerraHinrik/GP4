

#include "Units/Unit_Neutral.h"

#include "AI/AI_StateBase.h"
#include "AI/AI_StateMachine.h"
#include "AI/AI_State_Patrol.h"
#include "GameBoard/GameBoardUtils.h"
#include "GameBoard/Tiles/HexTile.h"


void AUnit_Neutral::BeginPlay()
{
	Super::BeginPlay();

	StateMachine = NewObject<UAI_StateMachine>();
	
	if (StateMachine)
	{
		TObjectPtr<UAI_State_Patrol> patrolState = NewObject<UAI_State_Patrol>();
		StateMachine->GetStateStack().Add(patrolState);
	}

	//SetupPatrolArea();
	

	
}

void AUnit_Neutral::SetupPatrolArea()
{
	//get all hexes on our ring
	TObjectPtr<UHexTile> current = Cast<UHexTile>(CurrentTile);
	TArray<TObjectPtr<UHexTile>> hexRing;
	if (current)
	{
		hexRing = GameBoardUtils::GetHexTileRing(current);
	}
	
	if (hexRing.IsEmpty())
		return;

	// get the upper or lower half of the ring (excluding start tiles) as your patrol area
	switch (hexRing.Num())
	{
		case 6:
			PatrolArea = GetPartOfRing(hexRing, 0);
			break;
		case 12:
			PatrolArea = GetPartOfRing(hexRing, 1);
			break;
		case 18:
			PatrolArea = GetPartOfRing(hexRing, 2);
			break;
		default:
			GEngine->AddOnScreenDebugMessage( -1, 1.f, FColor::Red, "No Matching Hex Ring" );	
			break;
	}
	
}

TArray<TObjectPtr<UTileBase>> AUnit_Neutral::GetPartOfRing(TArray<TObjectPtr<UHexTile>> ring, int ringIndex)
{
	TArray<TObjectPtr<UTileBase>> outArray;

	if (!CurrentTile || ringIndex < 0)
		return outArray;

	//if inner-most ring, get entire ring
	if (ringIndex == 0)
	{
		for (TObjectPtr<UHexTile> hex : ring)
		{
			outArray.Add(hex);
		}
	}
	else
	{
		FHexCoordinates currentCoords = CurrentTile->GetCoordinates();
		int currentR = currentCoords.R;
		for (TObjectPtr<UHexTile> hex : ring)
		{
			// multiply our current tile R-coord with each tile in the ring
			// if the result is positive they are on the same half of the ring
			int hexR = hex->GetHexCoordinates().R;
			
			if (currentR * hexR <= 0)
				continue;

			//exclude player start tiles
			if (hexR <= -ringIndex || hexR >= ringIndex)
				outArray.Add(hex);
		}
	}
	
	if (outArray.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage( -1, 1.f, FColor::Red, "Couldn't Get Patrol Area" );	
	}
	
	return outArray;
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
	TObjectPtr<UAI_StateBase> state = StateMachine->GetCurrentState();
	if (state)
	{
		state->OnStateRunning();
	}
	else
	{
		bFinishedMyTurn = true;
	}

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



