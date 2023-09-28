// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameBoard.generated.h"

class UHighlightSystem;
class UHexTile_Influence;
class UHexTile_Creation;
class UTile_ClaimableHexTile;
class AUnitBase;
struct FHexCoordinates;
class UTileBase;
class UHexTile;

UCLASS()
class GP4_TEAM02_API AGameBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameBoard();
	
	// Place unit on tile
	static bool PlaceUnitOnTile(AUnitBase* Unit, UTileBase* Tile);

	// Construct the tile grid
	void ConstructTileGrid(int32 PlayerControlledTeamAmount, int32 AIControlledTeamAmount);
	
private:
	virtual void Tick(float DeltaTime) override;

	void ClearGameBoard();
	void ConstructHexagonalTileGrid(int32 PlayerControlledTeamAmount, int32 AIControlledTeamAmount);
	
	// Create and return a node tile
	TObjectPtr<UTileBase> CreateTileComponent(TSubclassOf<UTileBase> TileType, const FHexCoordinates& HexCoordinates);
	
	// Connect a node to its neighbors
	void ConnectNodeToNeighbors(UHexTile* Node) const;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// OnConstruction
	virtual void OnConstruction(const FTransform& Transform) override;

public:

	// Grid Width is the number of tiles from the center to the edge of the grid excluding the center tile
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 GridWidth = 7;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 GridMaxHeight = 5;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float TileSpacing = 100.0f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	TArray<TSubclassOf<UTileBase>> StandardNodeTypes;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	TArray<FHexCoordinates> ClaimableTileCoordinates;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	TSubclassOf<UTile_ClaimableHexTile>  ClaimableTileType;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	TArray<FHexCoordinates> InfluenceTileCoordinates;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	TSubclassOf<UHexTile_Influence> InfluenceTileType;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Teams")
	TArray<TSubclassOf<class ATeam>> TeamTypes;
	UPROPERTY( BlueprintReadOnly, Category = "Grid")
	TArray<UHexTile*> NodeTiles;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "GameManager")
	float TurnTime = 40.0f;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHighlightSystem> HighlightSystem;

	//---------------------------------------------------------------------------------------------------//
	// Debug
	//---------------------------------------------------------------------------------------------------//
	
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Debug" )
	bool bShowGridCoordinates = false;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Debug" )
	bool bShowLinks = false;
};
