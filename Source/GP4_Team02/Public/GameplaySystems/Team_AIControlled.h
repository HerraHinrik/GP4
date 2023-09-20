// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplaySystems/Team.h"
#include "Team_AIControlled.generated.h"

USTRUCT(BlueprintType)
struct FUnitAndSpawnCoordinatesPair
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Units")
	TSubclassOf<AUnitBase> UnitClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Units")
	FHexCoordinates HexCoordinates;

};

/**
 * 
 */
UCLASS()
class GP4_TEAM02_API ATeam_AIControlled : public ATeam
{
	GENERATED_BODY()
public:
	// Tuple of UnitType and SpawnLocation
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Units" )
	TArray<FUnitAndSpawnCoordinatesPair> UnitSpawnPair;

private:

	
};
