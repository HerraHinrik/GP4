
#pragma once

#include "CoreMinimal.h"
#include "AI/AI_StateBase.h"
#include "AI_State_Patrol.generated.h"

class AUnit_Neutral;
class UTileBase;

UCLASS()
class GP4_TEAM02_API UAI_State_Patrol : public UAI_StateBase
{
	GENERATED_BODY()

private:
	TObjectPtr<UTileBase> NextTile;
	bool bIncrementIndex = true;
	bool bFirstTimePatrolling = true;

	UFUNCTION()
	bool GotNextTile();

	UFUNCTION()
	void FirstCheck();
	
	UFUNCTION()
	bool CheckForEnemies();

	
public:
	virtual void OnStateBegin() override;
	virtual void OnStateRunning() override;
	virtual void OnStateExit() override;
};
