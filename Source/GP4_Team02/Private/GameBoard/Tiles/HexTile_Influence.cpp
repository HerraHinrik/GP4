// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoard/Tiles/HexTile_Influence.h"
#include "GameplaySystems/Team.h"
#include "GameplaySystems/Team_PlayerControlled.h"
#include "GameplaySystems/TWS_GameManager.h"

void UHexTile_Influence::ClaimTile(TObjectPtr<ATeam> NewClaimingTeam)
{
	if(!NewClaimingTeam) return;
	if(const TObjectPtr<ATeam_PlayerControlled> PlayerTeam = Cast<ATeam_PlayerControlled>(NewClaimingTeam))
	{
		PlayerTeam->AddVictoryPoints(iVictoryPointAwarded);
	}
}

void UHexTile_Influence::BeginPlay()
{
	Super::BeginPlay();

	if(UTWS_GameManager* GameManager = GetWorld()->GetSubsystem<UTWS_GameManager>())
	{
		GameManager->OnTurnChanged.AddDynamic(this, &UHexTile_Influence::ResetTile);
	}
}

void UHexTile_Influence::ResetTile()
{
	ClaimingTeam = nullptr;
}
