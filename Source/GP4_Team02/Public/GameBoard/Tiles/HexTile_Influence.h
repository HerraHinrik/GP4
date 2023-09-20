// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile_ClaimableHexTile.h"
#include "HexTile_Influence.generated.h"

class ATeam;
/**
 * 
 */
UCLASS()
class GP4_TEAM02_API UHexTile_Influence : public UTile_ClaimableHexTile
{
	GENERATED_BODY()
public:
	virtual void ClaimTile(TObjectPtr<ATeam> NewClaimingTeam) override;

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Influence" )
	int32 iVictoryPointAwarded = 1;
};

