#pragma once

#include <GameBoard/Tiles/TileBase.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Units/UnitAction.h"
#include "Units/UnitAction/MoveAction.h"
#include "Units/UnitAction/AttackAction.h"
#include "UnitBase.generated.h"

class UUnitCondition_Base;
class ATeam;
class UHexTile;
class UTexture2D;
class UUnitAction_Offensive;
class UUnitAction_Supportive;

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

	TObjectPtr<ATeam> myTeam;

	TObjectPtr<UMoveAction> moveAction;

	TObjectPtr<UAttackAction> attackAction;
	bool bCanAttack = true;

	TObjectPtr<UTileBase> CurrentTile;

	TArray<TObjectPtr<UUnitCondition_Base>> Conditions;
	
	virtual void BeginPlay() override;

	void ChangeCurrentHealth(int change);

	void SetUnitDead();

	void SetUnitAlive();

public:
	UPROPERTY(BlueprintAssignable)
	FOnUnitDeath OnUnitDeath;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Stats")
	int iCurrentHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Stats")
	int iAttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Stats")
	int iAttackRange = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Stats")
	float fMoveSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit Actions")
	TArray<UUnitAction*> UnitActions;

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
	bool CanAffordAction(int actionCost);

	UFUNCTION(BlueprintCallable)
	bool CanAttack() { return bCanAttack; }
	
	UFUNCTION(BlueprintCallable)
	void ConsumeActionPoints(int actionCost) { iUnitActionPoints -= actionCost;}
	
	UFUNCTION(BlueprintPure)
	int GetRemainingActionPoints() const { return iUnitActionPoints; }
	
	TObjectPtr<ATeam> GetTeam() const { return myTeam; }
	
	void SetTeam(TObjectPtr<ATeam> team) { myTeam = team; }
	
	TArray<TObjectPtr<UTileBase>> GetAdjacentTiles();

	void virtual ResetUnit();

	void UpdateConditions();

	void UsedAttack() { bCanAttack = false; }

	virtual void Tick(float DeltaTime) override;
};

