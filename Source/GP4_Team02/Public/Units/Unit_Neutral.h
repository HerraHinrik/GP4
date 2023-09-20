
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
	TObjectPtr<UAI_StateMachine> StateMachine;

	TObjectPtr<AUnitBase> TargetUnit = nullptr;
	TArray<TObjectPtr<UTileBase>> PatrolArea;
	
	bool bFinishedMyTurn = false;
	
	virtual void BeginPlay() override;

	void SetupPatrolArea();

	TArray<TObjectPtr<UTileBase>> GetPartOfRing(TArray<TObjectPtr<UHexTile>> ring, int ringIndex);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int iPatrolIndex = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCircularPatrol = false;

	UFUNCTION(BlueprintCallable)
	void ExecuteCurrentState();
	
	TArray<TObjectPtr<UTileBase>> GetPatrolArea() const { return PatrolArea; }
	

	TObjectPtr<AUnitBase> GetTargetUnit() const { return TargetUnit; }

	bool GetFinishedMyTurn() const { return bFinishedMyTurn; }

	void SetNewTarget(TObjectPtr<AUnitBase> target) { TargetUnit = target; }
	
	bool CheckTargetInRange();

	virtual void ResetUnit() override;

	
};
