

#include "Units/Unit_Devil_Exploder.h"

#include "Units/UnitAction/Exploder_ExplosionAction.h"

AUnit_Devil_Exploder::AUnit_Devil_Exploder()
{
	explosionAction = CreateDefaultSubobject<UExploder_ExplosionAction>(FName("Explode Action"));
	UnitActions.Add(explosionAction);
}

void AUnit_Devil_Exploder::SetUnitDead()
{
	explosionAction->StartAction(CurrentTile, this);
	
	Super::SetUnitDead();
}
