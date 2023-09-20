

#include "Units/UnitAction/ClaimTileAction.h"
#include "Units/UnitBase.h"
#include "GameBoard/Tiles/HexTile.h"
#include "GameBoard/Tiles/Tile_ClaimableHexTile.h"

void UClaimTileAction::StartAction(UTileBase* tile, AUnitBase* unit)
{
	//null checks
	if (!tile || !unit)
		return;

	//other checks
	if (!bCanPerformAction || !tile->GetOccupyingUnit())
		return;
	
	//cache values
	ClaimableTile = Cast<UTile_ClaimableHexTile>(tile);

	if (!ClaimableTile)
		return;

	// Start the action
	UUnitAction::StartAction(tile, unit);

	ExecuteAction();
}

void UClaimTileAction::ExecuteAction()
{
	Super::ExecuteAction();

	// Claim the tile
	ClaimableTile->ClaimTile(Action_Unit->GetTeam());

	EndAction();
}

void UClaimTileAction::EndAction()
{
	ClaimableTile = nullptr;
	UUnitAction::EndAction();
}
