

#include "Units/Unit_Angel_Healer.h"

AUnit_Angel_Healer::AUnit_Angel_Healer()
{
	healAction = CreateDefaultSubobject<UHealer_HealAction>(FName("Heal Action"));
	UnitActions.Add(healAction);
}
