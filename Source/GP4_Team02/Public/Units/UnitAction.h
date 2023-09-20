
#pragma once

#include "CoreMinimal.h"
#include "UnitAction.generated.h"

class UTWS_GameManager;
class AUnitBase;
class UTileBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionStarted, UTileBase*, tile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionCompleted);

UCLASS(BlueprintType)
class GP4_TEAM02_API UUnitAction : public UActorComponent
{
	GENERATED_BODY()

protected:
	TObjectPtr<UTWS_GameManager> GameManager;
	TObjectPtr<UTileBase> Action_Tile;
	TObjectPtr<AUnitBase> Action_Unit;
	FTimerHandle Action_TimerHandle;
	FTimerDelegate Action_TimerDelegate;
	bool bCanPerformAction = true;

	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void ExecuteAction();
	
	UFUNCTION()
	virtual void EndAction();
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnActionStarted OnActionStarted;
	UPROPERTY(BlueprintAssignable)
	FOnActionCompleted OnActionCompleted;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ActionTime = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int iActionCost = 1;
	
	UFUNCTION(BlueprintCallable)
	virtual void StartAction(UTileBase* tile, AUnitBase* unit);

	UFUNCTION(BlueprintCallable)
	FVector GetActingUnitPosition();

	UFUNCTION(BlueprintCallable)
	FVector GetActionTargetPosition();

	UFUNCTION(BlueprintCallable)
	virtual void SetCost(int cost) { iActionCost = cost; }

protected:
	float ActionTimer = 0.0f;
};
