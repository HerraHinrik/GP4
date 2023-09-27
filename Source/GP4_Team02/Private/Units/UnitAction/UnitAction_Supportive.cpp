

#include "Units/UnitAction/UnitAction_Supportive.h"

#include "Units/UnitBase.h"

void UUnitAction_Supportive::StartAction(UTileBase* tile, AUnitBase* unit)
{
	Super::StartAction(tile, unit);
	unit->UsedSupport();
}

void UUnitAction_Supportive::EndAction()
{
	Super::EndAction();

	targetAlly = nullptr;
}
