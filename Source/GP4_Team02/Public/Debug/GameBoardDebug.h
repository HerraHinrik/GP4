#pragma once

class GameBoardDebug
{
public:
	// Add debug text for the game board
	static void AddDebugCoordinatesForGameBoard(class AGameBoard* GameBoard, class UWorld* World);

	// Visualize the links between the tiles
	static void VisualizeLinksBetweenTiles(class AGameBoard* GameBoard, class UWorld* World);
};
