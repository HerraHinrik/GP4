// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexTile.h"
#include "HexTile_Creation.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnHighLightCreation, bool, bClaimed );

UCLASS()
class GP4_TEAM02_API UHexTile_Creation : public UHexTile
{
	GENERATED_BODY()

public:
	void AddConnectedTile(const TObjectPtr<UHexTile_Creation> NewConnectedTile) { ConnectedTiles.Add(NewConnectedTile); }
	void RemoveConnectedTile(const TObjectPtr<UHexTile_Creation> NewConnectedTile) { ConnectedTiles.Remove(NewConnectedTile); }
	TSet<TObjectPtr<UHexTile_Creation>> GetConnectedTiles() const { return ConnectedTiles; }

	void SetOwningTeam(const TObjectPtr<ATeam> NewOwningTeam) { OwningTeam = NewOwningTeam; }
	TObjectPtr<ATeam> GetOwningTeam() const { return OwningTeam; }

	FOnHighLightCreation OnHighLightCreation;
	
private:
	TSet<TObjectPtr<UHexTile_Creation>> ConnectedTiles;
	TObjectPtr<ATeam> OwningTeam;
};
