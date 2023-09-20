

#include "Units/UnitAction/UnitAction_Supportive.h"

void UUnitAction_Supportive::EndAction()
{
	Super::EndAction();

	targetAlly = nullptr;
}
