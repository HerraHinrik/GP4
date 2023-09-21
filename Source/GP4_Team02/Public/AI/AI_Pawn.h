
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
	float fDelayTimer;
	bool bCanAct = true;

	TObjectPtr<ATeam> AI_Team;
	TArray<TObjectPtr<AUnit_Neutral>> TurnStack;
	TObjectPtr<AUnit_Neutral> ActiveUnit;
	

	void StackMyUnits();
	void ExecuteTurn(float DeltaSeconds);
	void WaitAndAct(float DeltaSeconds);
	void PerformUnitAction();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void CheckIfMyTurn() override;
	
	UPROPERTY(EditAnywhere)
	float fDelayBetweenActions = 2.0f;
	
};
