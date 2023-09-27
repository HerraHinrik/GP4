

#include "Units/Unit_Neutral.h"

#include "AI/AI_StateBase.h"
#include "AI/AI_StateMachine.h"
#include "AI/AI_State_ChasePlayerUnit.h"
#include "AI/AI_State_Patrol.h"
#include "GameBoard/GameBoard.h"
#include "GameBoard/GameBoardUtils.h"
#include "GameBoard/Tiles/HexTile.h"
#include "GameBoard/Tiles/Tile_ClaimableHexTile.h"
#include "GameplaySystems/TWS_GameManager.h"
#include "Kismet/KismetSystemLibrary.h"


void AUnit_Neutral::BeginPlay()
{
	Super::BeginPlay();

	StateMachine = NewObject<UAI_StateMachine>();
	
	if (StateMachine)
	{
		StateMachine->SetControlledUnit(this);
		
		TObjectPtr<UAI_State_Patrol> patrolState = NewObject<UAI_State_Patrol>();
		patrolState->SetAIUnit(this);
		patrolState->Machine = StateMachine;
		StateMachine->StateStack.Add(patrolState);
	}
	const TObjectPtr<UTWS_GameManager> GameManager = GetWorld()->GetSubsystem<UTWS_GameManager>();
	if(GameManager)
	{
		GameManager->OnGameReady.AddDynamic(this, &AUnit_Neutral::SetupPatrolArea);
		GameManager->OnTurnChanged.AddDynamic(this, &AUnit_Neutral::OnTurnChanged);
	}
}

void AUnit_Neutral::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bMyTurnToAct)
	{
		if (bFinishedMyTurn)
		{
			bMyTurnToAct = false;
		}
		else
		{
			ExecuteCurrentState();
		}
	}
}

#pragma region PatrolArea

void AUnit_Neutral::SetupPatrolArea()
{
	TObjectPtr<UHexTile> current = Cast<UHexTile>(CurrentTile);
	if (!current) { return; }

	int rCoord = current->GetHexCoordinates().R;

	PatrolArea.Empty();
	
	if (FMath::Abs(rCoord) <= 1)
	{
		TArray<TObjectPtr<UHexTile>> hexRing = GameBoardUtils::GetHexTileRing(current);
		for (TObjectPtr<UHexTile> hex : hexRing)
		{
			PatrolArea.Add(hex);
		}
	}
	else
	{
		for (TObjectPtr<UHexTile> hex : CurrentTile->GetGameBoardParent()->NodeTiles)
		{
			if (hex->GetHexCoordinates().R == rCoord)
			{
				PatrolArea.Add(hex);
			}
		}
		
		SortPartolArea();
	}
}

void AUnit_Neutral::OldSetupPatrolArea()
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
			SortPartolArea();
			break;
		case 18:
			PatrolArea = GetPartOfRing(hexRing, 2);
			SortPartolArea();
			break;
		default:
			GEngine->AddOnScreenDebugMessage( -1, 1.f, FColor::Red, "No Matching Hex Ring" );	
			break;
	}
}

void AUnit_Neutral::SetupIndex()
{
	if (!CurrentTile)
		return;

	int outIndex = 0;
	const int length = PatrolArea.Num();
	
	for (int i = 0; i < length; i++)
	{
		if (PatrolArea[i] == CurrentTile)
		{
			iPatrolIndex = outIndex;
			break;
		}
		
		outIndex++;
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
			// UKismetSystemLibrary::DrawDebugSphere(GetWorld(), hex->GetWorldLocation() + FVector(0,0,100.0f), 10.0f, 12, FColor::Red, 10.0f, 5.0f);
			outArray.Add(hex);
		}
		bCircularPatrol = true;
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
				// UKismetSystemLibrary::DrawDebugSphere(GetWorld(), hex->GetWorldLocation() + FVector(0,0,100.0f), 10.0f, 12, FColor::Red, 10.0f, 5.0f);
				outArray.Add(hex);
		}
	}
	
	if (outArray.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage( -1, 1.f, FColor::Red, "Couldn't Get Patrol Area" );	
	}
	
	return outArray;
}

void AUnit_Neutral::SortPartolArea()
{
	if (PatrolArea.IsEmpty())
		return;

	TArray<TObjectPtr<UTileBase>> sortedArea;
	TArray<TObjectPtr<UTileBase>> unsortedArea = PatrolArea;

	for (int i = 0 ; i < PatrolArea.Num() ; i++)
	{
		TObjectPtr<UTileBase> nextTile;
		int qCoord = 50;
		int sCoord = -50;
		
		for (TObjectPtr<UTileBase> tile : unsortedArea)
		{
			TObjectPtr<UHexTile> hex = Cast<UHexTile>(tile);
			if (!hex)
				continue;

			if (hex->GetHexCoordinates().Q == qCoord)
			{
				if (hex->GetHexCoordinates().S > sCoord)
				{
					qCoord = hex->GetHexCoordinates().Q;
					sCoord = hex->GetHexCoordinates().S;
					nextTile = tile;	
				}
			}
			
			if (hex->GetHexCoordinates().Q < qCoord)
			{
				qCoord = hex->GetHexCoordinates().Q;
				sCoord = hex->GetHexCoordinates().S;
				nextTile = tile;
			}
		}

		if (nextTile)
		{
			sortedArea.Add(nextTile);
			unsortedArea.Remove(nextTile);
		}
	}

	if (sortedArea.Num() == PatrolArea.Num())
	{
		PatrolArea = sortedArea;
	}
}

#pragma endregion

bool AUnit_Neutral::CheckTargetInRange()
{
	//if there is no target we can't check if it's in range, can we ?
	if (!GetTargetUnit())
		return false;

	//if the target made it to safety, stop chasing it
	if (GetTargetUnit()->IsInSafeZone())
	{
		TargetUnit = nullptr;
		return false;		
	}
	
	//get all nodes within attack range
	TArray<TObjectPtr<UTileBase>> tiles;
	GameBoardUtils::FindNodesWithinRadius(GetCurrentTile(), iAttackRange, tiles);

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
	//if a new state is waiting to be pushed, push that state
	if (StateMachine->NextState)
	{
		StateMachine->StateStack.Add(StateMachine->NextState);
		StateMachine->NextState = nullptr;
	}

	//get the top state and start running that state
	TObjectPtr<UAI_StateBase> state = StateMachine->GetCurrentState();
	if (state)
	{
		state->OnStateRunning();
	}
	else
	{
		bFinishedMyTurn = true;
	}

	//if unit is out of actions end our turn
	if (iUnitActionPoints <= 0)
	{
		bFinishedMyTurn = true;
	}
}

void AUnit_Neutral::WasAttacked(AUnitBase* attacker)
{
	if (TargetUnit && TargetUnit == attacker)
		return;

	TargetUnit = attacker;
	TObjectPtr<UAI_State_ChasePlayerUnit> chaseState = NewObject<UAI_State_ChasePlayerUnit>();
	StateMachine->PushNewState(chaseState);
}

void AUnit_Neutral::ResetUnit()
{
	Super::ResetUnit();

	bFinishedMyTurn = false;

	bMyTurnToAct = false;
}

void AUnit_Neutral::OnTurnChanged()
{
	//Check if current tile is claimable
	if (TObjectPtr<UTile_ClaimableHexTile> Tile = Cast<UTile_ClaimableHexTile>(CurrentTile))
	{
		//Claim tile
		Tile->ClaimTile(myTeam);
	}
}



