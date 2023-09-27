
#pragma once

#include "CoreMinimal.h"
#include "Units/Unit_Angel.h"
#include "Units/UnitAction/Cherub_BuffAction.h"
#include "Unit_Angel_Cherub.generated.h"

class UCherub_BuffAction;

UCLASS()
class GP4_TEAM02_API AUnit_Angel_Cherub : public AUnit_Angel
{
	GENERATED_BODY()

public:
	AUnit_Angel_Cherub();

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCherub_BuffAction> buffAction;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Unit Stats")
	int iBuffDuration = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Unit Stats")
	int iBuffActionRange = 2;

	UPROPERTY(BlueprintReadWrite, Category = "Unit Stats")
	int iBuffAmount = 1;
	
	virtual UUnitAction_Supportive* GetSupportiveAction() override
	{ return buffAction; }
};
