#pragma once

#include "CoreMinimal.h"
#include "GameBoard/FHexCoordinates.h"

class ULink;
struct FHexCoordinates;
class UHexTile;
class UTileBase;
class AGameBoard;

// Define the six possible offsets from the center tile
class GP4_TEAM02_API GameBoardUtils 
{
public:
	// Convert hexagonal coordinates to world coordinates
	static FVector HexToWorld(const FHexCoordinates& hex, float Width, float Height, float TileSpacing);
	// Find a node tile by its hexagonal coordinates
	static UHexTile* FindNodeByHexCoordinates(const FHexCoordinates& HexCoordinates, TArray<UHexTile*> NodeTiles);
	// Get the next linked tile in a given direction
	static UHexTile* GetNextLinkedTile(UHexTile* StartTile, EHexDirection Direction);	
	// Find the closest node to a given world location
	static UTileBase* FindClosestNode(const FVector& WorldLocation, TArray<UTileBase*> NodeTiles, float MaxDistance = UE_MAX_FLT);
	// Find the closest node to a given world location
	static UHexTile* FindClosestHexTile(const FVector& WorldLocation, TArray<UHexTile*> NodeTiles, float MaxDistance = UE_MAX_FLT);
	// Return all nodes within a given radius of world distance of a given world location
	static TArray<UTileBase*> GetNodesWithinRadius(const FVector& WorldLocation, TArray<UTileBase*> NodeTiles, float Radius);
	// Return all nodes within a given radius of tiles of a given world location
	static void FindNodesWithinRadius(UTileBase* StartNode, int32 MaxDistance, TArray<UTileBase*>& NodesWithinRadius, const bool bIncludeMiddle = false);
	static void FindNodesWithinRadius(UHexTile* StartNode, int32 MaxDistance, TArray<UHexTile*>& NodesWithinRadius, const bool bIncludeMiddle = false);
	// Get reflected hexagonal coordinates of a given hexagonal coordinate on a given axis
	static FHexCoordinates ReflectHexQ(const FHexCoordinates& CenterHexCoordinates);
	static FHexCoordinates ReflectHexR(const FHexCoordinates& CenterHexCoordinates);
	static FHexCoordinates ReflectHexS(const FHexCoordinates& CenterHexCoordinates);
	
	// Pathfinding functions
	static TArray<TObjectPtr<ULink>> FindPathInHexGrid(const TObjectPtr<UHexTile> StartNode, const TObjectPtr<UHexTile> EndNode);
	static TArray<TObjectPtr<ULink>> FindPathInHexGrid(const TObjectPtr<UTileBase> StartNode, const TObjectPtr<UTileBase> EndNode);

	// Get a ring of tiles around thr hexgrid starting with a tile at the given coordinates
	static TArray<TObjectPtr<UHexTile>> GetHexTileRing(TObjectPtr<UHexTile> StartTile);
	
	// Functions to get the six corner tiles of a hexagonal tile
	static TArray<TObjectPtr<UHexTile>> GetCornerTiles(TArray<TObjectPtr<UHexTile>> NodeTiles);
	static TObjectPtr<UHexTile> GetCornerTile(EHexDirection CornerDirection, TArray<TObjectPtr<UHexTile>> NodeTiles);
	static TArray<FHexCoordinates> GetCornerHexCoordinates(const TArray<FHexCoordinates>& FHexArray);
	static FHexCoordinates GetCornerHexCoordinate(EHexDirection CornerDirection, const TArray<FHexCoordinates>& FHexArray);
	static TArray<FHexCoordinates> GetAdjacentHexCoordinates(FHexCoordinates CornerHex, TArray<FHexCoordinates> Array);

private:
	// Helper functions

	// Get distance between two hexagonal coordinates
	static int32 GetHexDistance(const FHexCoordinates& HexCoordinatesA, const FHexCoordinates& HexCoordinatesB); 

};
