

#include "Units/UnitAction/Exploder_ExplosionAction.h"

#include "Units/UnitBase.h"
#include "Units/Unit_Devil_Exploder.h"

void UExploder_ExplosionAction::StartAction(UTileBase* tile, AUnitBase* unit)
{
	//null checks
	if (!tile || !unit)
		return;

	//other checks
	if (!tile->GetOccupyingUnit())
		return;
	

	TObjectPtr<UTileBase> startTile =  unit->GetCurrentTile();
	TObjectPtr<AUnit_Devil_Exploder> exploder = Cast<AUnit_Devil_Exploder>(unit);
	if (!startTile || !exploder)
		return;

	//cache values
	Super::StartAction(tile, unit);

	//damage surrounding units
	for (TObjectPtr<UTileBase> adjacentTile : exploder->GetAdjacentTiles())
	{
		if (adjacentTile->GetOccupyingUnit())
		{
			adjacentTile->GetOccupyingUnit()->ReceiveDamage(exploder->iExplosionDamage);
		}
	}
	
	ExecuteAction();
}

void UExploder_ExplosionAction::ExecuteAction()
{
	Super::ExecuteAction();

	EndAction();
}

void UExploder_ExplosionAction::EndAction()
{
	Super::EndAction();
}
