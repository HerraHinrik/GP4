// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexTile.h"
#include "Tile_ClaimableHexTile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClaimTile);

class ATeam;
/**
 * 
 */
UCLASS()
class GP4_TEAM02_API UTile_ClaimableHexTile : public UHexTile
{
	GENERATED_BODY()
public:
	virtual void ClaimTile(TObjectPtr<ATeam> NewClaimingTeam);
	TObjectPtr<ATeam> GetClaimingTeam() const { return ClaimingTeam; }
	virtual void SetOccupyingUnit(AUnitBase* unit) override;
	
	UPROPERTY( BlueprintAssignable )
	FOnClaimTile OnClaimTileEvent;
	
protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Claimable Tile")
	void OnEnterClaimTile(bool bIsAlreadyOwned);
	
	TObjectPtr<ATeam> ClaimingTeam;
	

};
