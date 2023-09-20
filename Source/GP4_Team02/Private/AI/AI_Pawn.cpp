

#include "AI/AI_Pawn.h"
#include "GameplaySystems/TWS_GameManager.h"
#include "Units/Unit_Neutral.h"


void AAI_Pawn::BeginPlay()
{
	Super::BeginPlay();

	myTeam = GameManager->AssignAITeam();

	GameManager->OnTurnChanged.AddDynamic(this, &AAI_Pawn::CheckIfMyTurn);

}

void AAI_Pawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsMyTurn)
	{
		ExecuteTurn(DeltaSeconds);
	}
}

void AAI_Pawn::CheckIfMyTurn()
{
	Super::CheckIfMyTurn();

	if (bIsMyTurn)
	{
		StackMyUnits();
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
	for (TObjectPtr<AUnitBase> unit : AI_Team->GetUnits())
	{
		TObjectPtr<AUnit_Neutral> neutralUnit = Cast<AUnit_Neutral>(unit);
		if (neutralUnit)
			TurnStack.Add(neutralUnit);
	}
}
