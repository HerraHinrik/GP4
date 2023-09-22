
#pragma once

#include "CoreMinimal.h"
#include "GameplaySystems/TeamPawn.h"
#include "AI_Pawn.generated.h"

class AUnit_Neutral;
class ATeam;

UCLASS()
class GP4_TEAM02_API AAI_Pawn : public ATeamPawn
{
	GENERATED_BODY()

private:
	UPROPERTY()
	float fDelayTimer;
	UPROPERTY()
	bool bCanAct = false;
	
	UPROPERTY()
	TArray<TObjectPtr<AUnit_Neutral>> TurnStack;
	UPROPERTY()
	TObjectPtr<AUnit_Neutral> ActiveUnit;
	

	UFUNCTION()
	void StackMyUnits();
	UFUNCTION()
	void ExecuteTurn(float DeltaSeconds);
	UFUNCTION()
	void WaitAndAct(float DeltaSeconds);
	UFUNCTION()
	void PerformUnitAction();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void CheckIfMyTurn() override;
	
	UPROPERTY(EditAnywhere)
	float fDelayBetweenActions = 2.0f;
	
};
