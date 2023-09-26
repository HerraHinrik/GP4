
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplaySystems/Team.h"
#include "TWS_GameManager.generated.h"


class ATeamPawn;
class APlayerInputController;
class AGameBoard;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnChanged);


UCLASS(BlueprintType)
class GP4_TEAM02_API UTWS_GameManager : public UTickableWorldSubsystem
{
	GENERATED_BODY()

private:

	TObjectPtr<AGameBoard> GameBoard;

	TArray<TObjectPtr<ATeamPawn>> TeamPawns;
	
#pragma region Teams

	int TeamIndex = 0;
	int PlayerControllerIndex = 0;
	int AIControllerIndex = 0;

	UPROPERTY()
	TArray<ATeam*> TeamArray;
	UPROPERTY()
	TObjectPtr<ATeam> CurrentTeam;
public:
	int32 PlayerControlledTeamAmount;
	int32 AIControlledTeamAmount;

	UFUNCTION( BlueprintPure )
	TArray<ATeam*> GetTeamArray() const { return TeamArray; }
	
#pragma endregion

#pragma region Timers
private:
	bool bTurnTimerOn = false;
	float fTurnRemainingTime;
	float fTurnMaxTime = 5.0f;
	float fBetweenMaxTime = 3.0f;
	
	bool bGameIntroTimerOn = false;
	float fGameIntroTimer = 3.0f;
#pragma endregion

#pragma region TurnProperties
	enum GameTurnState
	{
		NullState,
		TurnState,
		
		BetweenState
	};
	
	GameTurnState activeState;


	// int16 iRemainingActionPoints = 4;
	int32 iRoundNumber = 1;
#pragma endregion
	
	
	FInputModeUIOnly UIInputs;
	FInputModeGameAndUI GameAndUIInputs;

	void CoinFlip();
	void TurnTimer(float DeltaTime);
	void GameIntro(float DeltaTime);
	void ToggleController(APlayerController controller, bool setOn);
	void ChangeState();
	void ChangeTurn();
	void PawnsCheckTurn();
	
	virtual void Tick(float DeltaTime) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UTWS_GameManager, STATGROUP_Tickables)
	}

public:
	UPROPERTY(BlueprintAssignable)
	FOnTurnChanged OnTurnChanged;
	UPROPERTY( BlueprintAssignable )
	FOnGameReady OnGameReady;
	UPROPERTY( BlueprintAssignable )
	FOnGameStart OnGameStart;
	UPROPERTY( BlueprintAssignable )
	FOnGameEnded OnGameEnded;
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int32 iVictoryPointsToWin = 5;
	
UFUNCTION(BlueprintCallable)
void EndTurn();
	
UFUNCTION(BlueprintCallable)
void InitializeGame(TArray<TSubclassOf<class ATeam>> TeamTypes);

UFUNCTION(BlueprintCallable)
void CheckForWin();


void AssignTeamToController(TObjectPtr<APlayerInputController> controller);
TObjectPtr<ATeam> AssignAITeam();
void AddPawnToArray(TObjectPtr<ATeamPawn> pawn);
	
#pragma region GetterFunctions

	// Get Current Game Board
	UFUNCTION()
	AGameBoard* GetGameBoard() const { return GameBoard; }
	
	//current team name
	UFUNCTION(BlueprintPure)
	ATeam* GetCurrentTeam() const { return CurrentTeam; }
	UFUNCTION(BlueprintPure)
	FString GetCurrentTeamName() const { return CurrentTeam->GetTeamName(); }

	//turn timer
	UFUNCTION(BlueprintPure)
	float GetTurnRemainingTime() const { return fTurnRemainingTime; }
	UFUNCTION(BlueprintPure)
	float GetTurnMaxTime() const { return fTurnMaxTime; }
	UFUNCTION(BlueprintPure)
	float GetBetweenMaxTime() const { return fBetweenMaxTime; }
	UFUNCTION(BlueprintPure)
	float GetTurnTimerPercentage() const { return fTurnRemainingTime / fTurnMaxTime; }

	UFUNCTION(BlueprintPure)
	int GetRoundNumber() const { return iRoundNumber; }
	// UFUNCTION(BlueprintPure)
	// int GetRemainingActionPoints() const { return iRemainingActionPoints; }
	
	UFUNCTION(BlueprintPure)
	bool GetTurnActive() const
	{
		if (activeState == TurnState)
		{
			return true;
		}
		return false;
	}
#pragma endregion

#pragma region SetterFunctions
	void SetTurnMaxTime(float newTime) { fTurnMaxTime = newTime; }
	void SetBetweenMaxTime(float newTime) { fBetweenMaxTime = newTime; }
	void SetGameBoard(AGameBoard* newGameBoard) { GameBoard = newGameBoard; }
#pragma endregion 
};



