

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
	}
	else
	{
		bCanAct = false;
		ActiveUnit = nullptr;
	}
}

void AAI_Pawn::PerformUnitAction()
{
	fDelayTimer = fDelayBetweenActions;
	ActiveUnit->bMyTurnToAct = true;
}

void AAI_Pawn::ExecuteTurn(float DeltaSeconds)
{
	if (TurnStack.IsEmpty())
	{
		if(!ActiveUnit || (ActiveUnit && ActiveUnit->bFinishedMyTurn))
		{
			GameManager->EndTurn();
			bCanAct = false;
			return;
		}
	}
	
	if (!ActiveUnit || ActiveUnit->bFinishedMyTurn)
	{
		if (!TurnStack.IsEmpty())
		{
			ActiveUnit = TurnStack.Pop();
			GEngine->AddOnScreenDebugMessage( -1, 10.f, FColor::Red, "New Unit" );
		}
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
	int32 loops = myTeam->GetUnits().Num();
	for (int32 i = 0; i < loops; ++i)
	{
		TObjectPtr<AUnit_Neutral> neutralUnit = Cast<AUnit_Neutral>(myTeam->GetUnits()[i]);
		if (neutralUnit)
		{
			neutralUnit->iTurnLoops = 0;
			TurnStack.Add(neutralUnit);
		}
	}
	bCanAct = true;

}
