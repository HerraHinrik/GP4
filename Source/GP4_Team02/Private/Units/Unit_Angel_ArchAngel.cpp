
#include "Units/Unit_Angel_ArchAngel.h"

AUnit_Angel_ArchAngel::AUnit_Angel_ArchAngel()
{
	pierceAction = CreateDefaultSubobject<UArchAngel_PierceAction>(FName("Pierce Action"));
	UnitActions.Add(pierceAction);
}
