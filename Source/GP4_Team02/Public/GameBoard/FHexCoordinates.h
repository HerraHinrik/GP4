
#pragma once

#include "CoreMinimal.h"
#include "Containers/HashTable.h"
#include "Containers/Array.h"
#include "FHexCoordinates.generated.h"

USTRUCT( BlueprintType )
struct
FHexCoordinates
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "HexCoordinates" )
	int32 Q = 0;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "HexCoordinates" )
	int32 R = 0;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "HexCoordinates" )
	int32 S = 0;
	
	FHexCoordinates() : Q(0), R(0), S(0) {}  // Default constructor
	FHexCoordinates(const int32 Q, const int32 R, const int32 S) : Q(Q), R(R), S(S) {}
	FHexCoordinates(const FVector& Coordinates) : Q(Coordinates.X), R(Coordinates.Y), S(Coordinates.Z) {}

	FHexCoordinates operator + (const FHexCoordinates& Other) const {
		return FHexCoordinates(Q + Other.Q, R + Other.R, S + Other.S);
	}

	FHexCoordinates operator - ( const FHexCoordinates& Other ) const {
		return FHexCoordinates( Q - Other.Q, R - Other.R, S - Other.S );
	}

	FHexCoordinates operator * (const float Scale ) const {
		return FHexCoordinates( Q * Scale, R * Scale, S * Scale );
	}

	FHexCoordinates operator / (const float Scale ) const {
		return FHexCoordinates( Q / Scale, R / Scale, S / Scale );
	}

	bool operator == ( const FHexCoordinates& Other ) const {
		return  Q == Other.Q && R == Other.R && S == Other.S ;
	}

	bool operator != ( const FHexCoordinates& Other ) const
	{
		return !(*this == Other);
	}

	FString ToString() const
	{
		return FString::Printf(TEXT("Q: %d, R: %d, S: %d"), Q, R, S);
	}
	
};

enum class EHexDirection
{
	TopRight,
	TopLeft,
	Left,
	BottomLeft,
	BottomRight,
	Right,
	None
};

// Hash function for FHexCoordinates
static uint32 GetTypeHash(const FHexCoordinates& HexCoord)
{
	return HashCombine(HashCombine(HexCoord.Q, HexCoord.R), HexCoord.S);
}

extern TArray<FHexCoordinates> PointyTopHexagonCorners;
extern TArray<FHexCoordinates> FlatTopHexagonCorners;
extern TMap<EHexDirection, FHexCoordinates> HexDirectionToDirectionVector;

// extern FVector2D PointyTopDirections[6];
// extern FVector2D FlatTopDirections[6];

extern FHexCoordinates FHexDirectionVectors[6];
