

#include "Units/UnitAction/UnitAction_Offensive.h"

#include "Units/UnitBase.h"

void UUnitAction_Offensive::StartAction(UTileBase* tile, AUnitBase* unit)
{
	Super::StartAction(tile, unit);
	unit->UsedAttack();
}

void UUnitAction_Offensive::EndAction()
{
	Super::EndAction();

	targetEnemy = nullptr;
}
