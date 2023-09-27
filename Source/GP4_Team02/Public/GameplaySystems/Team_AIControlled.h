// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameBoard/FHexCoordinates.h"
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
	virtual TObjectPtr<AUnitBase> SpawnUnit(TSubclassOf<AUnitBase> UnitType) override;
	virtual AUnitBase* SpawnUnit(int32 UnitTier) override;
	virtual TArray<TObjectPtr<AUnitBase>> SpawnStartUnits() override;

public:
	// Tuple of UnitType and SpawnLocation
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Units" )
	TArray<FUnitAndSpawnCoordinatesPair> UnitSpawnPair;

private:

	
};
