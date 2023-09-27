
#include "GameplaySystems/TWS_GameManager.h"

#include "GameBoard/GameBoard.h"
#include "GameplaySystems/PlayerInputController.h"
#include "GameplaySystems/Team.h"
#include "GameplaySystems/TeamPawn.h"
#include "GameplaySystems/Team_AIControlled.h"
#include "GameplaySystems/Team_PlayerControlled.h"
#include "Kismet/GameplayStatics.h"
#include "Units/UnitBase.h"


void UTWS_GameManager::OnWorldBeginPlay(UWorld& InWorld)
{
	TArray<TObjectPtr<AActor>> board;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameBoard::StaticClass(), board);

	if (!board.IsEmpty() && board.Num() > 0)
	{
		bGameIntroTimerOn = true;
	}
}

void UTWS_GameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameIntro(DeltaTime);
	TurnTimer(DeltaTime);
}

#pragma region GameSetup

void UTWS_GameManager::	InitializeGame(TArray<TSubclassOf<ATeam>> TeamTypes)
{
	// Get game board
	if(!GameBoard)
	{
		if(const TObjectPtr<AGameBoard> GB = Cast<AGameBoard>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameBoard::StaticClass())))
		{
			GameBoard = GB;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No GameBoard found! Could not initialize game!"));
			return;
		}
	}

	
	// Spawn teams
	for (TSubclassOf<ATeam> TeamType : TeamTypes)
	{
		// Spawn team and pass it to the appropriate setup function
		const TObjectPtr<ATeam> Team = GetWorld()->SpawnActor<ATeam>(TeamType);
		// Try casting to player-controlled team
		if(const TObjectPtr<ATeam_PlayerControlled> PlayerControlledTeam = Cast<ATeam_PlayerControlled>(Team); PlayerControlledTeam)
		{
			PlayerControlledTeamAmount++;
		}
		else if(const TObjectPtr<ATeam_AIControlled> AIControlledTeam = Cast<ATeam_AIControlled>(Team); AIControlledTeam)
		{
			AIControlledTeamAmount++;
		}
		TeamArray.Add(Team);
	}

	// Construct tile grid
	GameBoard->ConstructTileGrid(PlayerControlledTeamAmount, AIControlledTeamAmount);

	// Place units on board
	for (const TObjectPtr<ATeam> Team : TeamArray)
	{
		Team->SpawnStartUnits();
	}
	
	// Set current team
	// CoinFlip();
	CurrentTeam = TeamArray[0];

	OnGameReady.Broadcast();
}

void UTWS_GameManager::CoinFlip()
{
	// If there are no teams, return
	if(TeamArray.IsEmpty())
		return;
	

	// Shuffle the array
	const int32 LastIndex = TeamArray.Num() - 1;
	for (int32 i = 0; i <= LastIndex; ++i)
	{
		const int32 Index = FMath::RandRange(i, LastIndex);
		if (i != Index)
		{
			TeamArray.Swap(i, Index);
		}
	}

	// Create a temporary array to store player-controlled teams
	TArray<TObjectPtr<ATeam>> PlayerControlledTeamsArray;
	// Find and collect player-controlled teams
	for (int32 i = 0; i < TeamArray.Num(); i++)
	{
		if(const TObjectPtr<ATeam_PlayerControlled> PlayerControlledTeam = Cast<ATeam_PlayerControlled>(TeamArray[i]); PlayerControlledTeam)
		{
			PlayerControlledTeamsArray.Add(PlayerControlledTeam);
		}
	}
	
	// Remove player-controlled teams from the original array
	for (const TObjectPtr<ATeam>& PlayerTeam : PlayerControlledTeamsArray)
	{
		TeamArray.Remove(PlayerTeam);
	}

	// Insert player-controlled teams at the start of the original array
	for (const TObjectPtr<ATeam>& PlayerTeam : PlayerControlledTeamsArray)
	{
		TeamArray.Insert(PlayerTeam, 0);
	}
	
	//flipping coin (kind of)
	const int Flip = FMath::RandRange(0,PlayerControlledTeamsArray.Num()- 1);

	// Get Team at index flip and put it at the start of the array
	const TObjectPtr<ATeam> TeamToMove = TeamArray[Flip];
	TeamArray.RemoveAt(Flip);
	TeamArray.Insert(TeamToMove, 0);

	//set current team
	CurrentTeam = TeamArray[0];
}

void UTWS_GameManager::GameIntro(float DeltaTime)
{
	if (bGameIntroTimerOn)
	{
		fGameIntroTimer -= DeltaTime;
		if (fGameIntroTimer <= 0)
		{
			bGameIntroTimerOn = false;
			activeState = TurnState;
			// PawnsCheckTurn();
			OnTurnChanged.Broadcast();
			bTurnTimerOn = true;
			if(CurrentTeam)
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, CurrentTeam->GetTeamName());
			OnGameStart.Broadcast();
		}
		fTurnRemainingTime = fTurnMaxTime;
	}
}

#pragma endregion 

#pragma region TurnManagement

void UTWS_GameManager::ChangeState()
{
	switch (activeState)
	{
	case TurnState:
		bTurnTimerOn = false;
		fTurnRemainingTime = fBetweenMaxTime;
		activeState = BetweenState;
		ChangeTurn();
		bTurnTimerOn = true;
		break;
	case BetweenState:
		bTurnTimerOn = false;
		fTurnRemainingTime = fTurnMaxTime;
		for (TObjectPtr<AUnitBase> Unit : CurrentTeam->GetUnits())
		{
			Unit->ResetUnit();
		}
		activeState = TurnState;
		bTurnTimerOn = true;
		break;
	default:
		break;
	}
}

void UTWS_GameManager::ChangeTurn()
{
	//do we only have one (or zero) teams?
	if (TeamArray.Num() < 2)
		return;


	//increment the index and set new active team
	TeamIndex++;
	
	if (TeamIndex >= TeamArray.Num())
	{
		TeamIndex = 0;
		iRoundNumber++;
	}
	CurrentTeam = TeamArray[TeamIndex];

	// PawnsCheckTurn();
	OnTurnChanged.Broadcast();
}

void UTWS_GameManager::PawnsCheckTurn()
{
	if (TeamPawns.IsEmpty())
		return;
	
	for (TObjectPtr<ATeamPawn> team : TeamPawns)
	{
		team->CheckIfMyTurn();
	}
}

void UTWS_GameManager::TurnTimer(float DeltaTime)
{
	if (bTurnTimerOn)
	{
		fTurnRemainingTime -= DeltaTime;
		if (fTurnRemainingTime <= 0)
		{
			ChangeState();
		}
	}
}

void UTWS_GameManager::EndTurn()
{
	if (activeState == TurnState)
		ChangeState();
}

void UTWS_GameManager::CheckForWin()
{
	for (const TObjectPtr<ATeam> Team : TeamArray)
	{
		if(const TObjectPtr<ATeam_PlayerControlled> PlayerTeam = Cast<ATeam_PlayerControlled>(Team))
		{
			if(PlayerTeam->GetVictoryPoints() >= iVictoryPointsToWin)
			{
				OnGameEnded.Broadcast();
				FString Winner = PlayerTeam->GetTeamName();
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Winner is: " + Winner);
				return;
			}
		}
	}
}

#pragma endregion 

void UTWS_GameManager::AssignTeamToController(TObjectPtr<APlayerInputController> controller)
{
	int maxControllers = 0;
	
	for (int16 i = 0; i < TeamArray.Num(); i++)
	{
		if(const TObjectPtr<ATeam_PlayerControlled> PlayerControlledTeam = Cast<ATeam_PlayerControlled>(TeamArray[i]); PlayerControlledTeam)
		{
			maxControllers++;
		}
		else
		{
			break;
		}
	}
	
	if (PlayerControllerIndex < maxControllers)
	{
		controller->AssignTeam(TeamArray[PlayerControllerIndex]);
		PlayerControllerIndex++;
	}
}

TObjectPtr<ATeam> UTWS_GameManager::AssignAITeam()
{
	TObjectPtr<ATeam_AIControlled> outTeam = Cast<ATeam_AIControlled>(TeamArray.Last());

	if (!outTeam)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "No AI Team");
	}
	
	return outTeam;
}

void UTWS_GameManager::AddPawnToArray(TObjectPtr<ATeamPawn> pawn)
{
	if (pawn)
	{
		TeamPawns.Add(pawn);
	}
}


void UTWS_GameManager::ToggleController(APlayerController controller, bool setOn)
{
	//if setOn is true, enable all inputs
	if (setOn)
	{
		controller.SetInputMode(GameAndUIInputs);
	}
	else
	{
		controller.SetInputMode(UIInputs);
	}
}
