

#include "Units/Unit_Devil_Imp.h"


AUnit_Devil_Imp::AUnit_Devil_Imp()
{
	poisonAction = CreateDefaultSubobject<UImp_PoisonAction>(FName("Poison Action"));
	UnitActions.Add(poisonAction);
}
