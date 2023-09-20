// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexTile.h"
#include "HexTile_Creation.generated.h"



UCLASS(  )
class GP4_TEAM02_API UHexTile_Creation : public UHexTile
{
	GENERATED_BODY()

public:
	void AddConnectedTile(const TObjectPtr<UHexTile_Creation> NewConnectedTile) { ConnectedTiles.Add(NewConnectedTile); }

private:
		TSet<TObjectPtr<UHexTile_Creation>> ConnectedTiles;
};
