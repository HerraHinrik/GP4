#include "GameBoard/GameBoardUtils.h"
#include "GameBoard/Tiles/HexTile.h"
#include "GameBoard/Link.h"
#include "GameBoard/FHexCoordinates.h"
#include "GameBoard/GameBoard.h"

FVector GameBoardUtils::HexToWorld(const FHexCoordinates& hex, float Width, float Height, float TileSpacing)
{
	float X = (Width + TileSpacing) * sqrt(3.0f) * (hex.Q + hex.R / 2.0f);
	float Y = (Height + TileSpacing) * 3.0f / 2.0f * hex.R;
	return FVector(X, Y, 0.0f);
}

UHexTile* GameBoardUtils::FindNodeByHexCoordinates(const FHexCoordinates& HexCoordinates, TArray<UHexTile*> NodeTiles)
{
	// Loop through the array of HexTiles and find the one with matching coordinates
	for (UHexTile* Tile : NodeTiles)
	{
		const FHexCoordinates HexTileHexCoordinates = Tile->GetHexCoordinates();
		if (HexTileHexCoordinates == HexCoordinates)
		{
			return Tile; // Found a matching HexTile
		}
	}

	// Return nullptr if no matching HexTile is found
	return nullptr;
}

UHexTile* GameBoardUtils::GetNextLinkedTile(UHexTile* StartTile, EHexDirection Direction)
{
	// Calculate the position of the node in the corresponding direction
	const FHexCoordinates NextNodeHexCoordinates = StartTile->GetHexCoordinates() + HexDirectionToDirectionVector[Direction];
	// Loop through all links and return the target of the link that matches the given direction
	for (const ULink* Link : StartTile->GetLinks())
	{
		UHexTile* Target = Cast<UHexTile>(Link->GetTarget());
		if (Target && Target->GetHexCoordinates() == NextNodeHexCoordinates)
		{
			return Target;
		}
	}
	return nullptr;
}

UTileBase* GameBoardUtils::FindClosestNode(const FVector& WorldLocation, TArray<UTileBase*> NodeTiles, float MaxDistance)
{
	// Return nullptr if the array is empty
	if( NodeTiles.IsEmpty() ) return nullptr;

	// Loop through the array of nodes and compare distances while keeping track of the closest node
	UTileBase* ClosestNode = nullptr;
	for (UTileBase* NodeTile : NodeTiles)
	{
		const float Distance = FVector::Dist(WorldLocation, NodeTile->GetWorldLocation());
		if (Distance < MaxDistance)
		{
			ClosestNode = NodeTile;
			MaxDistance = Distance;
		}
	}
	return ClosestNode;
}

UHexTile* GameBoardUtils::FindClosestHexTile(const FVector& WorldLocation, TArray<UHexTile*> HexTiles, float MaxDistance)
{
	// Return nullptr if the array is empty
	if( HexTiles.IsEmpty() ) return nullptr;

	// Loop through the array of nodes and compare distances while keeping track of the closest node
	UHexTile* ClosestTile = nullptr;
	for (UHexTile* HexTile : HexTiles)
	{
		const float Distance = FVector::Dist(WorldLocation, HexTile->GetWorldLocation());
		if (Distance < MaxDistance)
		{
			ClosestTile = HexTile;
			MaxDistance = Distance;
		}
	}
	return ClosestTile;
}

TArray<UTileBase*> GameBoardUtils::GetNodesWithinRadius(const FVector& WorldLocation, TArray<UTileBase*> NodeTiles, float Radius)
{
	// Return an empty array if the array is empty
	if( NodeTiles.IsEmpty() ) return TArray<UTileBase*>();

	// Loop through the array of nodes collecting all nodes within the given radius
	TArray<UTileBase*> NodesWithinRadius;
	for (UTileBase* NodeTile : NodeTiles)
	{
		const float Distance = FVector::Dist(WorldLocation, NodeTile->GetWorldLocation());
		if (Distance < Radius)
		{
			NodesWithinRadius.Add(NodeTile);
		}
	}

	return NodesWithinRadius;
}

void GameBoardUtils::FindNodesWithinRadius(UTileBase* StartNode, const int32 MaxDistance, TArray<UTileBase*>& NodesWithinRadius, const bool bIncludeMiddle)
{
	// Add the start node to the array of nodes within the radius
	if (bIncludeMiddle) NodesWithinRadius.Add(StartNode);
	
	TMap<UTileBase*, int32> Distances;
	TQueue<UTileBase*> Queue;

	Queue.Enqueue(StartNode);
	Distances.Add(StartNode, 0);

	while (!Queue.IsEmpty()) {
		UTileBase* CurrentNode;
		Queue.Dequeue(CurrentNode);

		// Loop through all neighbors of the current node
		for (const ULink* Link : CurrentNode->GetLinks()) {
			UTileBase* Neighbor = Link->GetTarget();
			
			// Check if the neighbor has already been visited
			if (Distances.Contains(Neighbor)) continue;
			
			// Add the neighbor to the queue
			Queue.Enqueue(Neighbor);
			// Add the neighbor to the distances map
			Distances.Add(Neighbor, Distances[CurrentNode] + 1);
			
			// Check if the neighbor is within the max distance
			if (Distances[Neighbor] <= MaxDistance) {
				// Add the neighbor to the array of nodes within the radius
				NodesWithinRadius.Add(Neighbor);
			}
			
		}

	}
}

void GameBoardUtils::FindNodesWithinRadius(UHexTile* StartNode, int32 MaxDistance, TArray<UHexTile*>& NodesWithinRadius, const bool bIncludeMiddle)
{
	// Cast the start node to a base node
	UTileBase* StartBaseNode = Cast<UTileBase>(StartNode);
	
	TArray<UTileBase*> BaseNodesWithinRadius;
	// Use the base node function to find nodes within radius
	FindNodesWithinRadius(StartBaseNode, MaxDistance, BaseNodesWithinRadius, bIncludeMiddle);

	// Cast the base nodes to hex nodes
	for (UTileBase* BaseNode : BaseNodesWithinRadius)
	{
		UHexTile* HexNode = Cast<UHexTile>(BaseNode);
		NodesWithinRadius.Add(HexNode);
	}
		
}

FHexCoordinates GameBoardUtils::ReflectHexQ(const FHexCoordinates& CenterHexCoordinates)
{
	return FHexCoordinates( CenterHexCoordinates.Q, CenterHexCoordinates.S, CenterHexCoordinates.R );
}

FHexCoordinates GameBoardUtils::ReflectHexR(const FHexCoordinates& CenterHexCoordinates)
{
	return FHexCoordinates( CenterHexCoordinates.S, CenterHexCoordinates.R, CenterHexCoordinates.Q );
}

FHexCoordinates GameBoardUtils::ReflectHexS(const FHexCoordinates& CenterHexCoordinates)
{
	return FHexCoordinates( CenterHexCoordinates.R, CenterHexCoordinates.Q, CenterHexCoordinates.S );
}

TArray<TObjectPtr<ULink>> GameBoardUtils::FindPathInHexGrid(const TObjectPtr<UHexTile> StartNode, const TObjectPtr<UHexTile> EndNode)
{
	// Create an empty array of nodes to return
	TArray<TObjectPtr<ULink>> Path;

	// Early out if either start and end are null or if they are the same or if the node tiles array is empty
	if (StartNode == nullptr || EndNode == nullptr || StartNode == EndNode ) return Path;

	// Create Closed and Open
	TSet<TObjectPtr<UHexTile>> Closed;
	TArray<TObjectPtr<UHexTile>> Open;

	// Add the start node to Open
	StartNode->GCost = 0;
	StartNode->HCost = GetHexDistance(StartNode->GetHexCoordinates(), EndNode->GetHexCoordinates());
	Open.Add(StartNode);

	// Search for the end node
	int32 Iterations = 0; // Sanity check
	while (!Open.IsEmpty() && Iterations < 10000)
	{
		Iterations++;
		
		// Get the node with the lowest F cost
		TObjectPtr<UHexTile> CurrentNode = Open[0];
		for (TObjectPtr<UHexTile> OpenNode : Open)
		{
			if (OpenNode->FCost < CurrentNode->FCost)
			{
				CurrentNode = OpenNode;
			}
		}
		Open.Remove(CurrentNode);
		Closed.Add(CurrentNode);

		// Check if the current node is the end node and return reconstructed path
		if (CurrentNode == EndNode)
		{
			// Reconstruct the path			
			while (CurrentNode->ParentNode != nullptr)
			{
				Path.Add( CurrentNode->ParentNode->GetLinkTo(CurrentNode));
				CurrentNode = Cast<UHexTile>(CurrentNode->ParentNode);
			}
			// Reset A* values in all nodes
			for (TObjectPtr<UHexTile> Node : Closed)
			{
				Node->ResetAStarVariables();
			}
			for (UHexTile* HexTile : Open)
			{
				HexTile->ResetAStarVariables();
			}	
			// Return the path
			Algo::Reverse(Path);
			return Path;
		}

		// Loop through all neighbors of the current node
		for (const ULink* Link : CurrentNode->GetLinks())
		{
			// Cast the link target to a hex node and skip if the cast fails or if the neighbor is in the closed set
			TObjectPtr<UHexTile> Neighbor = Cast<UHexTile>(Link->GetTarget());
			if (!Neighbor) continue;
			if(Neighbor->GetOccupyingUnit()) continue;
			if (Closed.Contains(Neighbor)) continue;

			// Calculate the new G cost and update the neighbor if it's not in the open set
			const float NewGCost = CurrentNode->GCost + GetHexDistance(CurrentNode->GetHexCoordinates(), Neighbor->GetHexCoordinates());
			if (NewGCost < Neighbor->GCost || !Open.Contains(Neighbor))
			{
				Neighbor->GCost = NewGCost;
				Neighbor->HCost = GetHexDistance(Neighbor->GetHexCoordinates(), EndNode->GetHexCoordinates());
				Neighbor->FCost = Neighbor->GCost + Neighbor->HCost;
				Neighbor->ParentNode = CurrentNode;

				if (!Open.Contains(Neighbor))
				{
					Open.Add(Neighbor);
				}
			}
		}
	}

    // No path found
    return Path;
}

TArray<TObjectPtr<ULink>> GameBoardUtils::FindPathInHexGrid(const TObjectPtr<UTileBase> StartNode, const TObjectPtr<UTileBase> EndNode)
{
	// Cast the start and end nodes to hex nodes
	const TObjectPtr<UHexTile> StartHexNode = Cast<UHexTile>(StartNode);
	const TObjectPtr<UHexTile> EndHexNode = Cast<UHexTile>(EndNode);

	return FindPathInHexGrid(StartHexNode, EndHexNode);
}

TArray<TObjectPtr<UHexTile>> GameBoardUtils::GetCornerTiles(TArray<TObjectPtr<UHexTile>> NodeTiles)
{
	TArray<TObjectPtr<UHexTile>> CornerTiles;
	CornerTiles.Add(GetCornerTile(EHexDirection::TopRight, NodeTiles));
	CornerTiles.Add(GetCornerTile(EHexDirection::TopLeft, NodeTiles));
	CornerTiles.Add(GetCornerTile(EHexDirection::Left, NodeTiles));
	CornerTiles.Add(GetCornerTile(EHexDirection::BottomLeft, NodeTiles));
	CornerTiles.Add(GetCornerTile(EHexDirection::BottomRight, NodeTiles));
	CornerTiles.Add(GetCornerTile(EHexDirection::Right, NodeTiles));
	
	return CornerTiles;
}

TObjectPtr<UHexTile> GameBoardUtils::GetCornerTile(const EHexDirection CornerDirection, TArray<TObjectPtr<UHexTile>> NodeTiles)
{
	TArray<FHexCoordinates> HexCoordinates;
	for (TObjectPtr<UHexTile> NodeTile : NodeTiles)
	{
		HexCoordinates.Add(NodeTile->GetHexCoordinates());
	}
	FHexCoordinates CornerCoordinate = GetCornerHexCoordinate(CornerDirection, HexCoordinates);
	return FindNodeByHexCoordinates(CornerCoordinate, NodeTiles);
}

TArray<FHexCoordinates> GameBoardUtils::GetCornerHexCoordinates(const TArray<FHexCoordinates>& FHexArray)
{
	TArray<FHexCoordinates> CornerTiles;
	CornerTiles.Add(GetCornerHexCoordinate(EHexDirection::TopRight, FHexArray));
	CornerTiles.Add(GetCornerHexCoordinate(EHexDirection::TopLeft, FHexArray));
	CornerTiles.Add(GetCornerHexCoordinate(EHexDirection::Left, FHexArray));
	CornerTiles.Add(GetCornerHexCoordinate(EHexDirection::BottomLeft, FHexArray));
	CornerTiles.Add(GetCornerHexCoordinate(EHexDirection::BottomRight, FHexArray));
	CornerTiles.Add(GetCornerHexCoordinate(EHexDirection::Right, FHexArray));
	
	return CornerTiles;
}

FHexCoordinates GameBoardUtils::GetCornerHexCoordinate(EHexDirection CornerDirection,	const TArray<FHexCoordinates>& FHexArray)
{
	FHexCoordinates CornerCoordinate;
	if(FHexArray.IsEmpty()) return CornerCoordinate;
	
	FHexCoordinates StartingNodeCoordinates = FHexCoordinates(0,0,0);
	// Calculate the height and width of the grid
	int32 GridWidth = 0;
	int32 GridHeight = 0;
	
	for (const FHexCoordinates Coordinates : FHexArray)
	{
		if( FMath::Abs(Coordinates.R) > GridHeight )
		{
			GridHeight = FMath::Abs(Coordinates.R);
		}
		if( Coordinates.R == 0 && FMath::Abs(Coordinates.Q) > GridWidth )
		{
			GridWidth = FMath::Abs(Coordinates.Q);
		}
	}
	// Calculate the offset to the starting node
	const int32 GridStartOffset = GridWidth - GridHeight;

	FHexCoordinates Direction = FHexCoordinates(0,0,0);

	switch (CornerDirection)
	{
	case EHexDirection::TopRight:
		Direction = FHexDirectionVectors[0];
		// Shuffle GridStartOffset in the right direction
		StartingNodeCoordinates = StartingNodeCoordinates + FHexDirectionVectors[5] * GridStartOffset;
		break;
	case EHexDirection::TopLeft:
		Direction = FHexDirectionVectors[1];
		// Shuffle GridStartOffset in the left direction
		StartingNodeCoordinates = StartingNodeCoordinates + FHexDirectionVectors[2] * GridStartOffset;
		break;
	case EHexDirection::Left:
		Direction = FHexDirectionVectors[2];
		break;
	case EHexDirection::BottomLeft:
		Direction = FHexDirectionVectors[3];
		// Shuffle GridStartOffset in the left direction
		StartingNodeCoordinates = StartingNodeCoordinates + FHexDirectionVectors[2] * GridStartOffset;
		break;
	case EHexDirection::BottomRight:
		Direction = FHexDirectionVectors[4];
		// Shuffle GridStartOffset in the right direction
		StartingNodeCoordinates = StartingNodeCoordinates + FHexDirectionVectors[5] * GridStartOffset;
		break;
	case EHexDirection::Right:
		Direction = FHexDirectionVectors[5];
		break;
	default:
		break;
	}

	// Continue searching for the top right tile until a nullptr is found
	int32 Iterations = 0; // Sanity check
	while (Iterations < 100)
	{
		// Get the hex coordinates of the tile to the top right of the current tile
		const FHexCoordinates NextCornerTileCoordinates = CornerCoordinate + Direction;
		// Check if the tile is valid
		if (!FHexArray.Contains(NextCornerTileCoordinates))
			break;
		CornerCoordinate = NextCornerTileCoordinates;

		// Increment the iterations
		Iterations++;
	}

	return CornerCoordinate;
}

TArray<FHexCoordinates> GameBoardUtils::GetAdjacentHexCoordinates(FHexCoordinates CornerHex, TArray<FHexCoordinates> Array)
{
	TArray<FHexCoordinates> AdjacentHexCoordinates;
	for(int32 i = 0; i < PointyTopHexagonCorners.Num(); i++)
	{
		const FHexCoordinates AdjacentHexCoordinate = CornerHex + PointyTopHexagonCorners[i];
		if(Array.Contains(AdjacentHexCoordinate))
		{
			AdjacentHexCoordinates.Add(AdjacentHexCoordinate);
		}
	}
	return AdjacentHexCoordinates;
}

int32 GameBoardUtils::GetHexDistance(const FHexCoordinates& HexCoordinatesA, const FHexCoordinates& HexCoordinatesB)
{
	FHexCoordinates HexCoordinateVector = HexCoordinatesA - HexCoordinatesB;
	return (abs(HexCoordinateVector.Q) + abs(HexCoordinateVector.R) + abs(HexCoordinateVector.S)) / 2;
}

TArray<TObjectPtr<UHexTile>> GameBoardUtils::GetHexTileRing(TObjectPtr<UHexTile> StartTile)
{
	TArray<TObjectPtr<UHexTile>> HexTileRing;
	HexTileRing.Add(StartTile);
	const FHexCoordinates StartHexCoordinates = StartTile->GetHexCoordinates();

	int32 RingRadius = FMath::Abs(StartHexCoordinates.Q) > FMath::Abs(StartHexCoordinates.R) ? FMath::Abs(StartHexCoordinates.Q) : FMath::Abs(StartHexCoordinates.R);
	RingRadius = FMath::Abs(StartHexCoordinates.S) > RingRadius ? FMath::Abs(StartHexCoordinates.S) : RingRadius;

	TArray<ULink*> Links = StartTile->GetLinks();
	while(!Links.IsEmpty())
	{
		TObjectPtr<ULink> Current = Links.Pop();
		TObjectPtr<UHexTile> Target = Cast<UHexTile>(Current->GetTarget());
		if(!Target) continue;
		if(HexTileRing.Contains(Target)) continue;

		const FHexCoordinates TargetHexCoordinates = Target->GetHexCoordinates();
		int32 TargetRingRadius = FMath::Abs(TargetHexCoordinates.Q) > FMath::Abs(TargetHexCoordinates.R) ? FMath::Abs(TargetHexCoordinates.Q) : FMath::Abs(TargetHexCoordinates.R);
		TargetRingRadius = FMath::Abs(TargetHexCoordinates.S) > TargetRingRadius ? FMath::Abs(TargetHexCoordinates.S) : TargetRingRadius;

		if( TargetRingRadius != RingRadius ) continue;
		
		HexTileRing.Add(Target);
		Links.Append(Target->GetLinks());
	}
	
	return HexTileRing;
}




