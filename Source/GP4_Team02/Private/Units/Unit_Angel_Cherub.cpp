

#include "Units/Unit_Angel_Cherub.h"


AUnit_Angel_Cherub::AUnit_Angel_Cherub()
{
	buffAction = CreateDefaultSubobject<UCherub_BuffAction>(FName("Buff Action"));
	UnitActions.Add(buffAction);
}
