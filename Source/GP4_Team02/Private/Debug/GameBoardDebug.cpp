#include "Debug/GameBoardDebug.h"

#include "GameBoard/GameBoard.h"
#include "GameBoard/Link.h"
#include "GameBoard/Tiles/HexTile.h"
#include "Kismet/KismetSystemLibrary.h"

void GameBoardDebug::AddDebugCoordinatesForGameBoard(AGameBoard* GameBoard, UWorld* World)
{
	// Null check
	if(!World || !GameBoard || GameBoard->NodeTiles.IsEmpty()) return;
	
	// Visualize the hex coordinates
	for (UHexTile* NodeTile : GameBoard->NodeTiles)
	{
		const FHexCoordinates Hex = NodeTile->GetHexCoordinates();
		UKismetSystemLibrary::DrawDebugString(World, NodeTile->GetWorldLocation(), FString::Printf(TEXT("%d, %d, %d"), Hex.Q, Hex.R, Hex.S), nullptr, FColor::Blue, 100000.0f);
	}
}

void GameBoardDebug::VisualizeLinksBetweenTiles(AGameBoard* GameBoard, UWorld* World)
{
	// Null check
	if(!World || !GameBoard || GameBoard->NodeTiles.IsEmpty()) return;

	// Visualize the links
	for (UTileBase* NodeTile : GameBoard->NodeTiles)
	{
		for (const ULink* Link : NodeTile->GetLinks()) {
			FVector Start = Link->GetSource()->GetWorldLocation();
			FVector End = Link->GetTarget()->GetWorldLocation();
			UKismetSystemLibrary::DrawDebugLine(World, Start + FVector(0,0, 50.0f), End + FVector(0,0, 50.0f), FColor::Red, 100000.0f);
		}
	}
}


