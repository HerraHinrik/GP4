
#pragma once

#include "CoreMinimal.h"
#include "Units/Unit_Angel.h"
#include "Units/UnitAction/ArchAngel_PierceAction.h"
#include "Unit_Angel_ArchAngel.generated.h"

class UArchAngel_PierceAction;

UCLASS()
class GP4_TEAM02_API AUnit_Angel_ArchAngel : public AUnit_Angel
{
	GENERATED_BODY()

public:
	AUnit_Angel_ArchAngel();

private:
	TObjectPtr<UArchAngel_PierceAction> pierceAction;

public:
	virtual UUnitAction_Offensive* GetOffensiveAction() override
	{ return pierceAction; }
};
