// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexTile.h"
#include "Tile_ClaimableHexTile.generated.h"

class ATeam;
/**
 * 
 */
UCLASS()
class GP4_TEAM02_API UTile_ClaimableHexTile : public UHexTile
{
	GENERATED_BODY()
public:
	virtual void ClaimTile(TObjectPtr<ATeam> NewClaimingTeam);
	TObjectPtr<ATeam> GetClaimingTeam() const { return ClaimingTeam; }
	
private:
	TObjectPtr<ATeam> ClaimingTeam;
};
