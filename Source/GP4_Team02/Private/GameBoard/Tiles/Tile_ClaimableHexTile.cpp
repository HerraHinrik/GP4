// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoard/Tiles/Tile_ClaimableHexTile.h"

#include "GameplaySystems/Team.h"

void UTile_ClaimableHexTile::ClaimTile(TObjectPtr<ATeam> NewClaimingTeam)
{
	if(!NewClaimingTeam) return;
	
	if(ClaimingTeam)
	{
		ClaimingTeam->RemoveClaimedTile(this);
	}
	ClaimingTeam = NewClaimingTeam;
	ClaimingTeam->AddClaimedTile(this);
	
}
