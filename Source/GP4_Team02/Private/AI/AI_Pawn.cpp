

#include "AI/AI_Pawn.h"
#include "GameplaySystems/TWS_GameManager.h"
#include "Units/Unit_Neutral.h"


void AAI_Pawn::BeginPlay()
{
	Super::BeginPlay();

	myTeam = GameManager->AssignAITeam();
	GameManager->AddPawnToArray(this);
	
}

void AAI_Pawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bCanAct)
	{
		ExecuteTurn(DeltaSeconds);
	}
}

void AAI_Pawn::CheckIfMyTurn()
{
	if (!myTeam)
	{
		myTeam = GameManager->AssignAITeam();
	}
	
	Super::CheckIfMyTurn();

	if (bIsMyTurn)
	{
		StackMyUnits();
		bCanAct = true;
	}
	else
	{
		bCanAct = false;
	}
}

void AAI_Pawn::PerformUnitAction()
{
	fDelayTimer = fDelayBetweenActions;
	ActiveUnit->ExecuteCurrentState();
}

void AAI_Pawn::ExecuteTurn(float DeltaSeconds)
{
	if (TurnStack.IsEmpty())
		GameManager->EndTurn();
	
	if (!ActiveUnit || ActiveUnit->GetFinishedMyTurn())
	{
		if (!TurnStack.IsEmpty())
			ActiveUnit = TurnStack.Pop();
	}

	if (ActiveUnit)
		WaitAndAct(DeltaSeconds);
}

void AAI_Pawn::WaitAndAct(float DeltaSeconds)
{
	fDelayTimer -= DeltaSeconds;
	if (fDelayTimer <= 0)
	{
		PerformUnitAction();
	}
}

void AAI_Pawn::StackMyUnits()
{
	TurnStack.Empty();
	for (TObjectPtr<AUnitBase> unit : myTeam->GetUnits())
	{
		TObjectPtr<AUnit_Neutral> neutralUnit = Cast<AUnit_Neutral>(unit);
		if (neutralUnit)
			TurnStack.Add(neutralUnit);
	}
}
