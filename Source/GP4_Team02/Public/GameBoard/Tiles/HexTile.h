// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameBoard/Tiles/TileBase.h"
#include "GameBoard/FHexCoordinates.h"
#include "HexTile.generated.h"



UCLASS(  )
class GP4_TEAM02_API UHexTile : public UTileBase
{
	GENERATED_BODY()

public:
	
	// Return Height based on Width
	virtual float GetHeight() const override { return TileWidth * sqrt(3.0f) / 2.0f; }

	// Calculate and return Height based on Width
	virtual float GetWidth() const override { return TileWidth * sqrt(3.0f) / 2.0f; }

	// Get/Set hex
	FHexCoordinates GetHexCoordinates() const { return HexCoordinates; }
	void SetHexCoordinates(FHexCoordinates hex) { HexCoordinates = hex; }

	UFUNCTION(BlueprintCallable)
	virtual FVector GetCoordinates() const override { return FVector(HexCoordinates.Q, HexCoordinates.R, HexCoordinates.S); }
	UFUNCTION(BlueprintCallable)
	virtual void SetCoordinates(FVector Coordinates) override { SetHexCoordinates(FHexCoordinates(Coordinates.X, Coordinates.Y, Coordinates.Z)); }


private:
		FHexCoordinates HexCoordinates;
};
