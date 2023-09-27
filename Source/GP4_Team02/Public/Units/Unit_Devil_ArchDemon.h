
#pragma once

#include "CoreMinimal.h"
#include "Units/Unit_Devil.h"
#include "Units/UnitAction/ArchDemon_SwipeAction.h"
#include "Unit_Devil_ArchDemon.generated.h"

class UArchDemon_SwipeAction;

UCLASS()
class GP4_TEAM02_API AUnit_Devil_ArchDemon : public AUnit_Devil
{
	GENERATED_BODY()

public:
	AUnit_Devil_ArchDemon();

private:
	TObjectPtr<UArchDemon_SwipeAction> swipeAction;

public:
	
	virtual UUnitAction_Offensive* GetOffensiveAction() override
	{ return swipeAction; }
	
};
