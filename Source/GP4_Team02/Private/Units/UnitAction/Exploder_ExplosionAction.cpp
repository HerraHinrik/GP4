

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
	exploder = Cast<AUnit_Devil_Exploder>(unit);
	if (!startTile || !exploder)
		return;

	//damage surrounding units
	unit->GetAdjacentTiles(explosionRadius);
	if (explosionRadius.IsEmpty())
		return;
	
	//cache values, start action
	Super::StartAction(tile, unit);
	
	ExecuteAction();
}

void UExploder_ExplosionAction::ExecuteAction()
{
	for (TObjectPtr<UTileBase> adjacentTile : explosionRadius)
	{
		if (adjacentTile->GetOccupyingUnit())
		{
			adjacentTile->GetOccupyingUnit()->ReceiveDamage(exploder->iExplosionDamage);
		}
	}

	if (Action_Tile->GetOccupyingUnit() != Action_Unit)
	{
		Action_Tile->GetOccupyingUnit()->ReceiveDamage(exploder->iExplosionDamage);
	}
	
	EndAction();
}

void UExploder_ExplosionAction::EndAction()
{
	Super::EndAction();
}
