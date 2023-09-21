
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
	virtual TObjectPtr<AUnitBase> SpawnUnit(TSubclassOf<AUnitBase> UnitType);
	UFUNCTION( BlueprintCallable, Category = "Units")
	virtual AUnitBase* SpawnUnit(int32 UnitTier = 0);
	virtual TArray<TObjectPtr<AUnitBase>> SpawnStartUnits();

	// Add/Remove a tile from the list of claimed tiles
	void AddClaimedTile(const TObjectPtr<UTile_ClaimableHexTile> Tile) { ClaimedTiles.Add(Tile); }
	void RemoveClaimedTile(const TObjectPtr<UTile_ClaimableHexTile> Tile) { ClaimedTiles.Remove(Tile); }

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
	
	UPROPERTY(BlueprintReadOnly, Category = "TeamUnits" )
	TArray<TObjectPtr<AUnitBase>> Units;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TeamUnits" )
	int32 iMaxSpawnPoints = 1;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TeamUnits" )
	int32 MaxUnitsSpawned;


	UPROPERTY(BlueprintAssignable)
	FOnUnitsChanged OnUnitsChanged;
	UPROPERTY( BlueprintAssignable )
	FOnVictoryPointsChanged OnVictoryPointsChanged;
	UFUNCTION()
	void RemoveDeadUnits();
	TArray<TObjectPtr<AUnitBase>> GetUnits() const { return Units; }

protected:
	TObjectPtr<UTWS_GameManager> GameManager;
	UFUNCTION()
	virtual void OnTurnChanged();
	virtual void BeginPlay() override;
	
private:
	void AllUnitsDead();
	
	TArray<TObjectPtr<AUnitBase>> UnitPool;
};

