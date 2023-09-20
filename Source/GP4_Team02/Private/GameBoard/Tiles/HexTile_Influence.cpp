// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoard/Tiles/HexTile_Influence.h"
#include "GameplaySystems/Team.h"

void UHexTile_Influence::ClaimTile(TObjectPtr<ATeam> NewClaimingTeam)
{
	if(!NewClaimingTeam) return;
	NewClaimingTeam->AddVictoryPoints(iVictoryPointAwarded);
}
