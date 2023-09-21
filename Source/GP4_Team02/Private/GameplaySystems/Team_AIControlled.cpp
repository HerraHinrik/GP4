#include "GameplaySystems/Team_AIControlled.h"
#include "GameBoard/GameBoard.h"
#include "GameBoard/GameBoardUtils.h"
#include "GameBoard/Tiles/HexTile.h"
#include "GameplaySystems/TWS_GameManager.h"
#include "Units/UnitBase.h"

TObjectPtr<AUnitBase> ATeam_AIControlled::SpawnUnit(TSubclassOf<AUnitBase> UnitType)
{
	if(Units.Num() >= MaxUnitsSpawned ) return nullptr;

	const TObjectPtr<AUnitBase> Unit = Super::SpawnUnit(UnitType);
	return Unit;
}

AUnitBase* ATeam_AIControlled::SpawnUnit(int32 UnitTier)
{
	if(	UnitSpawnPair.Num() <= 0 ) return nullptr;
	// Clamp the tier to the number of unit types
	UnitTier = FMath::Clamp(UnitTier, 0, UnitSpawnPair.Num() - 1);
	return SpawnUnit(UnitSpawnPair[UnitTier].UnitClass);
}

TArray<TObjectPtr<AUnitBase>> ATeam_AIControlled::SpawnStartUnits()
{
	const TArray<UHexTile*>& NodeTiles = GameManager->GetGameBoard()->NodeTiles;
	
	TArray<TObjectPtr<AUnitBase>> SpawnedUnits;
	for (FUnitAndSpawnCoordinatesPair& Pair : UnitSpawnPair)
	{
		TObjectPtr<AUnitBase> Unit = SpawnUnit(Pair.UnitClass);
		SpawnedUnits.Add(Unit);
		if(const TObjectPtr<UHexTile> Tile = GameBoardUtils::FindNodeByHexCoordinates(Pair.HexCoordinates, NodeTiles))
		{
			AGameBoard::PlaceUnitOnTile(Unit, Tile);
		}
	}
	
	return SpawnedUnits;
}