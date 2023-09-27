
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
	UPROPERTY(EditAnywhere)
	TObjectPtr<UImp_PoisonAction> poisonAction;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Unit Stats")
	int iPoisonDuration = 2;
	
	virtual UUnitAction_Offensive* GetOffensiveAction() override
	{ return poisonAction; }

};
