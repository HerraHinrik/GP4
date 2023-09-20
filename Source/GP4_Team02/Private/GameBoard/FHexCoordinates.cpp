#include "GameBoard/FHexCoordinates.h"

// Define the corners of a pointy-top hexagon
TArray<FHexCoordinates> PointyTopHexagonCorners = {
	FHexCoordinates(1, -1, 0), 
	FHexCoordinates(1, 0, -1), 
	FHexCoordinates(0, 1, -1), 
	FHexCoordinates(-1, 1, 0), 
	FHexCoordinates(-1, 0, 1), 
	FHexCoordinates(0, -1, 1) 
};

// Define the corners of a flat-top hexagon
TArray<FHexCoordinates> FlatTopHexagonCorners = {
	FHexCoordinates(1, 0, -1),
	FHexCoordinates(0, 1, -1),
	FHexCoordinates(-1, 1, 0),
	FHexCoordinates(-1, 0, 1),
	FHexCoordinates(0, -1, 1),
	FHexCoordinates(1, -1, 0)
};

// Convert hexagonal coordinates to world coordinates
FHexCoordinates FHexDirectionVectors[6] = {
	FHexCoordinates(+1, -1, 0),		// Top Right
	FHexCoordinates(0, -1, +1),		// Top Left
	FHexCoordinates(-1, 0, +1),		// Left
	FHexCoordinates(-1, +1, 0),		// Bottom Left
	FHexCoordinates(0, +1, -1),		// Bottom Right
	FHexCoordinates(+1, 0, -1)		// Right
};

// Map corner directions to hexagonal coordinates
TMap<EHexDirection, FHexCoordinates> HexDirectionToDirectionVector = {
	{ EHexDirection::TopRight, FHexCoordinates(+1, -1, 0) },
	{ EHexDirection::TopLeft, FHexCoordinates(0, -1, +1) },
	{ EHexDirection::Left, FHexCoordinates(-1, 0, +1) },
	{ EHexDirection::BottomLeft, FHexCoordinates(-1, +1, 0) },
	{ EHexDirection::BottomRight, FHexCoordinates(0, +1, -1) },
	{ EHexDirection::Right, FHexCoordinates(+1, 0, -1) },
	{ EHexDirection::None, FHexCoordinates(0, 0, 0) }
};