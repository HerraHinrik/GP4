// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexTile.h"
#include "GameBoard/GameBoard.h"
#include "GameBoard/HighlightSystem.h"
#include "HexTile_Creation.generated.h"

class ATeam;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHighLightCreation, bool, bClaimed);

UCLASS()
class GP4_TEAM02_API UHexTile_Creation : public UHexTile
{
	GENERATED_BODY()

public:
	virtual void AddLink(ULink* link) override;
	
	void AddConnectedTile(const TObjectPtr<UHexTile_Creation> NewConnectedTile)
	{
		if(ConnectedTiles.Contains(NewConnectedTile))
		{
			return;
		}
		ConnectedTiles.Add(NewConnectedTile);
		// Make sure the other tile knows about this tile as well
		NewConnectedTile->AddConnectedTile(this);
	}
	TArray<TObjectPtr<UHexTile_Creation>> GetConnectedTiles() const { return ConnectedTiles; }

	void SetOwningTeam(const TObjectPtr<ATeam> NewOwningTeam) { OwningTeam = NewOwningTeam; }
	TObjectPtr<ATeam> GetOwningTeam() const { return OwningTeam; }
	
	virtual void SetGameBoardParent(const TObjectPtr<AGameBoard> GameBoard) override { Super::SetGameBoardParent(GameBoard); GameBoard->HighlightSystem->AddCreationTile(this); }


	UPROPERTY(BlueprintAssignable)
	FOnHighLightCreation OnHighLightCreation;
	
private:
	TArray<TObjectPtr<UHexTile_Creation>> ConnectedTiles;
	TObjectPtr<ATeam> OwningTeam;
};


