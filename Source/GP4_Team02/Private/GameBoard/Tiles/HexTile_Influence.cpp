// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoard/Tiles/HexTile_Influence.h"
#include "GameplaySystems/Team.h"
#include "GameplaySystems/Team_PlayerControlled.h"

void UHexTile_Influence::ClaimTile(TObjectPtr<ATeam> NewClaimingTeam)
{
	if(!NewClaimingTeam) return;
	if(const TObjectPtr<ATeam_PlayerControlled> PlayerTeam = Cast<ATeam_PlayerControlled>(NewClaimingTeam))
	{
		PlayerTeam->AddVictoryPoints(iVictoryPointAwarded);
	}
}
