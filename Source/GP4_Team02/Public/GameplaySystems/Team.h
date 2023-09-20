
#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "GameBoard/FHexCoordinates.h"
#include "GameBoard/Tiles/TileBase.h"
#include "Team.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnitsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVictoryPointsChanged);

class UHexTile_Creation;
class UTWS_GameManager;
class UTile_ClaimableHexTile;
class AUnitBase;

UCLASS(BlueprintType)
class GP4_TEAM02_API ATeam : public AActor
{
GENERATED_BODY()
	
public:
	// int iMaxActionPoints;
	// int iRemainingActionPoints;
	
	FString GetTeamName() const { return TeamName; }
	void SetTeamName(FString Name) { TeamName = Name; }
	
	TObjectPtr<AUnitBase> GetUnitAtIndex(const int UnitIndex) { return Units[UnitIndex]; }

	// Spawn a unit of a specific type
	TObjectPtr<AUnitBase> SpawnUnit(TSubclassOf<AUnitBase> UnitType);
	UFUNCTION( BlueprintCallable, Category = "Units")
	AUnitBase* SpawnUnit(int32 UnitTier = 0);

	// Add/Remove a tile from the list of claimed tiles
	void AddClaimedTile(const TObjectPtr<UTile_ClaimableHexTile> Tile) { ClaimedTiles.Add(Tile); }
	void RemoveClaimedTile(const TObjectPtr<UTile_ClaimableHexTile> Tile) { ClaimedTiles.Remove(Tile); }

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
	
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Team" )
	FString TeamName = "NoName";
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Team" )
	TArray<TSubclassOf<AUnitBase>> UnitTypes;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Team" )
	int32 iUnitStartAmount;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Team" )
	TSubclassOf<UHexTile_Creation> CreationTileType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ClaimedTiles" )
	TArray<TObjectPtr<UTile_ClaimableHexTile>> ClaimedTiles;
	
	UPROPERTY(BlueprintReadOnly, Category = "Units" )
	TArray<TObjectPtr<AUnitBase>> Units;
	UPROPERTY( BlueprintReadWrite, Category = "Units" )
	int32 iMaxSpawnPoints = 1;

	UPROPERTY(BlueprintAssignable)
	FOnUnitsChanged OnUnitsChanged;
	UPROPERTY( BlueprintAssignable )
	FOnVictoryPointsChanged OnVictoryPointsChanged;
	
	void RemoveDeadUnits();
	TArray<TObjectPtr<AUnitBase>> GetUnits() const { return Units; }

protected:
	TObjectPtr<UTWS_GameManager> GameManager;
UFUNCTION()
	void OnTurnChanged();
	virtual void BeginPlay() override;
	
private:
	void AllUnitsDead();
	
	TArray<TObjectPtr<AUnitBase>> UnitPool;
	TArray<TObjectPtr<UHexTile_Creation>> CreationTiles;
	TArray<FHexCoordinates> CreationTileCoordinates;
	int32 iSpawnPoints;
	int32 iVictoryPoints;
};

