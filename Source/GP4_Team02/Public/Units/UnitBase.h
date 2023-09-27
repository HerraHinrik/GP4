#pragma once

#include <GameBoard/Tiles/TileBase.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitAction/ClaimTileAction.h"
#include "Units/UnitAction.h"
#include "Units/UnitAction/MoveAction.h"
#include "Units/UnitAction/AttackAction.h"
#include "UnitBase.generated.h"

class UUnitAction_Supportive;
class UUnitCondition_Base;
class ATeam;
class UHexTile;
class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPoisonTick, UTileBase*, tile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnitDeath);


UCLASS(BlueprintType)
class GP4_TEAM02_API AUnitBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AUnitBase();
	
protected:
	bool bIsAlive = true;
	int iUnitActionPoints = 0;

	UPROPERTY()
	TObjectPtr<ATeam> myTeam;
	UPROPERTY()
	TObjectPtr<UMoveAction> moveAction;
	UPROPERTY()
	TObjectPtr<UAttackAction> attackAction;
	UPROPERTY()
	bool bCanAttack = true;
	UPROPERTY()
	bool bCanSupport = true;
	UPROPERTY()
	TObjectPtr<UClaimTileAction> claimTileAction;
	UPROPERTY()
	TObjectPtr<UTileBase> CurrentTile;
	UPROPERTY()
	TArray<TObjectPtr<UUnitCondition_Base>> Conditions;

	
	virtual void BeginPlay() override;

	UFUNCTION()
	void ChangeCurrentHealth(int change);
	UFUNCTION()
	 virtual void SetUnitDead();
	UFUNCTION()
	virtual void SetUnitAlive();

public:
	UPROPERTY(BlueprintAssignable)
	FOnUnitDeath OnUnitDeath;

	UPROPERTY(BlueprintAssignable)
	FOnPoisonTick OnPoisonTick;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Settings")
	TObjectPtr<UTexture2D> portrait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Settings")
	bool bRetaliateWhenAttacked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Settings")
	FString unitType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Settings")
	float fHeightOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Stats")
	int iMaxActionPoints = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Stats")
	int iMaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit Stats")
	int iCurrentHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Stats")
	int iAttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Stats")
	int iAttackRange = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Stats")
	float fMoveSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit Actions")
	TArray<UUnitAction*> UnitActions;

	UPROPERTY()
	TArray<TObjectPtr<UTileBase>> AdjacentTiles;

	UFUNCTION(BlueprintPure)
	bool IsUnitAlive() const { return bIsAlive; }
	
	UFUNCTION(BlueprintPure)
	UTileBase* GetCurrentTile() const { return CurrentTile; }

	UFUNCTION(BlueprintCallable)
	void SetCurrentTile(UTileBase* newTile)
	{
		if(CurrentTile)
			CurrentTile->RemoveUnit();
		CurrentTile = newTile;
	}
	
	UFUNCTION(BlueprintPure)
	TArray<UUnitCondition_Base*> GetConditions() const { return Conditions; }

	UFUNCTION(BlueprintCallable)
	void AddCondition(UUnitCondition_Base* condition, int duration, int potency);

	UFUNCTION(BlueprintCallable)
	void RemoveCondition(UUnitCondition_Base* condition);

	UFUNCTION(BlueprintCallable)
	bool IsInSafeZone();

	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(int amount);

	UFUNCTION(BlueprintCallable)
	void ReceiveHealing(int amount);

	UFUNCTION(BlueprintCallable)
	UMoveAction* GetMoveAction() const {return moveAction;}

	UFUNCTION(BlueprintCallable)
	virtual UUnitAction_Offensive* GetOffensiveAction() {return attackAction;}

	UFUNCTION(BlueprintCallable)
	virtual UUnitAction_Supportive* GetSupportiveAction() {return nullptr;}

	UFUNCTION(BlueprintCallable)
	UClaimTileAction* GetClaimTileAction() {return claimTileAction;}

	UFUNCTION(BlueprintCallable)
	bool CanAffordAction(int actionCost);
	
	UFUNCTION(BlueprintCallable)
	void ConsumeActionPoints(int actionCost) { iUnitActionPoints -= actionCost;}
	
	UFUNCTION(BlueprintPure)
	int GetRemainingActionPoints() const { return iUnitActionPoints; }
	
	TObjectPtr<ATeam> GetTeam() const { return myTeam; }
	
	void SetTeam(TObjectPtr<ATeam> team) { myTeam = team; }

	UFUNCTION()
	void GetAdjacentTiles(TArray<UTileBase*>& tileArray);

	void virtual ResetUnit();

	UFUNCTION()
	void UpdateConditions();

	UFUNCTION(BlueprintCallable)
	bool CanAttack() { return bCanAttack; }
	
	UFUNCTION()
	void UsedAttack() { bCanAttack = false; }

	UFUNCTION(BlueprintCallable)
	bool CanSupport() { return bCanSupport; }
	
	UFUNCTION()
	void UsedSupport() { bCanSupport = false; }

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnActionCompleted();
};

