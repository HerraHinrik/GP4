// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplaySystems/Team.h"
#include "Team_PlayerControlled.generated.h"

/**
 * 
 */
UCLASS()
class GP4_TEAM02_API ATeam_PlayerControlled : public ATeam
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
	// Spawn Functions
	virtual TObjectPtr<AUnitBase> SpawnUnit(TSubclassOf<AUnitBase> UnitType) override;
	virtual AUnitBase* SpawnUnit(int32 UnitTier) override;
	virtual TArray<TObjectPtr<AUnitBase>> SpawnStartUnits() override;


	// Add/retrieve the creation tile/coordinates
	void AddCreationTile(const TObjectPtr<UHexTile_Creation> Tile);
	void AppendCreationTiles(const TArray<TObjectPtr<UHexTile_Creation>>& Tiles);
	void AddCreationTileCoordinates(const FHexCoordinates& Coordinates);
	void AppendCreationTileCoordinates(const TArray<FHexCoordinates>& Coordinates);
	TArray<TObjectPtr<UHexTile_Creation>> GetCreationTiles() const { return CreationTiles; }
	TArray<FHexCoordinates> GetCreationTileCoordinates() const { return CreationTileCoordinates; }

	// Get CreationTileType
	TSubclassOf<UHexTile_Creation> GetCreationTileType() const { return CreationTileType; }

	// Add VictoryPoints
	void AddVictoryPoints(const int32 Amount) { iVictoryPoints += Amount; OnVictoryPointsChanged.Broadcast(); }
	// Get VictoryPoints
	int32 GetVictoryPoints() const { return iVictoryPoints; }

private:
	TArray<TObjectPtr<UHexTile_Creation>> CreationTiles;
	TArray<FHexCoordinates> CreationTileCoordinates;
	int32 iVictoryPoints;

};
