
#pragma once

#include "CoreMinimal.h"
#include "Units/Unit_Angel.h"
#include "Units/UnitAction/Healer_HealAction.h"
#include "Unit_Angel_Healer.generated.h"

class UHealer_HealAction;

UCLASS()
class GP4_TEAM02_API AUnit_Angel_Healer : public AUnit_Angel
{
	GENERATED_BODY()

public:
	AUnit_Angel_Healer();

private:
	TObjectPtr<UHealer_HealAction> healAction;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Unit Stats")
	int iHealAmount = 2;

	UPROPERTY(BlueprintReadWrite, Category = "Unit Stats")
	int iHealActionRange = 2;
	
	virtual UUnitAction_Supportive* GetSupportiveAction() override
	{ return healAction; }

	virtual UUnitAction_Offensive* GetOffensiveAction() override
	{ return nullptr; }
	
};
