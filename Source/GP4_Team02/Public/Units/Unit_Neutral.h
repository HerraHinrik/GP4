
#pragma once

#include "CoreMinimal.h"
#include "Units/UnitBase.h"
#include "Unit_Neutral.generated.h"

class AUnitBase;
class UAI_StateMachine;

UCLASS()
class GP4_TEAM02_API AUnit_Neutral : public AUnitBase
{
	GENERATED_BODY()

	

protected:
	UPROPERTY()
	TObjectPtr<UAI_StateMachine> StateMachine;

	UPROPERTY()
	TObjectPtr<AUnitBase> TargetUnit = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UTileBase>> PatrolArea;
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void SetupPatrolArea();


	TArray<TObjectPtr<UTileBase>> GetPartOfRing(TArray<TObjectPtr<UHexTile>> ring, int ringIndex);

	UFUNCTION()
	void SortPartolArea();

public:
	UPROPERTY()
	bool bFinishedMyTurn = false;

	UPROPERTY()
	bool bMyTurnToAct = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int iPatrolIndex = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCircularPatrol = false;

	UFUNCTION(BlueprintCallable)
	void ExecuteCurrentState();

	UFUNCTION()
	void SetupIndex();
	
	UFUNCTION(BlueprintCallable)
	void WasAttacked(AUnitBase* attacker);
	
	TArray<TObjectPtr<UTileBase>> GetPatrolArea() const { return PatrolArea; }
	

	TObjectPtr<AUnitBase> GetTargetUnit() const { return TargetUnit; }

	// bool GetFinishedMyTurn() const { return bFinishedMyTurn; }

	void SetNewTarget(TObjectPtr<AUnitBase> target) { TargetUnit = target; }

	UFUNCTION()
	bool CheckTargetInRange();

	UFUNCTION()
	virtual void ResetUnit() override;

	
};
