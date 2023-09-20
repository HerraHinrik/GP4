
#pragma once

#include "CoreMinimal.h"
#include "Units/Unit_Devil.h"
#include "Units/UnitAction/Imp_PoisonAction.h"
#include "Unit_Devil_Imp.generated.h"


UCLASS()
class GP4_TEAM02_API AUnit_Devil_Imp : public AUnit_Devil
{
	GENERATED_BODY()

public:
	AUnit_Devil_Imp();

private:
	TObjectPtr<UImp_PoisonAction> poisonAction;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Poison Action")
	int iPoisonDuration = 2;

	UPROPERTY(BlueprintReadWrite, Category = "Poison Action")
	int iPoisonActionRange = 2;

	UPROPERTY(BlueprintReadWrite, Category = "Poison Action")
	int iPoisonDamagePerTurn = 1;
	
	virtual UUnitAction_Offensive* GetOffensiveAction() override
	{ return poisonAction; }

};
