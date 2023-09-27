

#include "GameplaySystems/TeamPawn.h"

#include "GameplaySystems/TWS_GameManager.h"

ATeamPawn::ATeamPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}


void ATeamPawn::BeginPlay()
{
	Super::BeginPlay();
	
	GameManager = GetWorld()->GetSubsystem<UTWS_GameManager>();
	
	GameManager->OnTurnChanged.AddDynamic(this, &ATeamPawn::CheckIfMyTurn);
	
}

void ATeamPawn::CheckIfMyTurn()
{
	bIsMyTurn = GameManager->GetCurrentTeam() == myTeam;
	myTeam->UpdateUnitConditions();
	if (bIsMyTurn)
	{
		myTeam->bWasMyTurn = true;
	}
}

void ATeamPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATeamPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

