#include "GameplaySystems/Team_PlayerControlled.h"
#include "GameBoard/GameBoard.h"
#include "GameBoard/GameBoardUtils.h"
#include "GameBoard/Tiles/HexTile_Creation.h"
#include "GameplaySystems/TWS_GameManager.h"
#include "Units/UnitBase.h"

void ATeam_PlayerControlled::BeginPlay()
{
	Super::BeginPlay();
	
	// Set spawn points to the start units amount
	iSpawnPoints = iUnitStartAmount;
}

void ATeam_PlayerControlled::AddCreationTile(const TObjectPtr<UHexTile_Creation> Tile)
{
	if(!Tile) return;
	if(!CreationTiles.Contains(Tile))
		CreationTiles.Add(Tile);
	
	const FHexCoordinates Coordinates = Tile->GetHexCoordinates();
	if(!CreationTileCoordinates.Contains(Coordinates))
		AddCreationTileCoordinates(Coordinates);
}

void ATeam_PlayerControlled::AppendCreationTiles(const TArray<TObjectPtr<UHexTile_Creation>>& Tiles)
{
	for (TObjectPtr<UHexTile_Creation> Tile : Tiles)
	{
		AddCreationTile(Tile);
	}
}

void ATeam_PlayerControlled::AddCreationTileCoordinates(const FHexCoordinates& Coordinates)
{
	if(!CreationTileCoordinates.Contains(Coordinates))
		CreationTileCoordinates.Add(Coordinates);

	if(!GameManager) return;
	const TArray<UHexTile*> Tiles = GameManager->GetGameBoard()->NodeTiles;
	if( Tiles.Num() == 0 ) return;
	if(const TObjectPtr<UHexTile_Creation> CreationTile =  Cast<UHexTile_Creation>(GameBoardUtils::FindNodeByHexCoordinates(Coordinates, Tiles)))
		AddCreationTile(CreationTile);

}

void ATeam_PlayerControlled::AppendCreationTileCoordinates(const TArray<FHexCoordinates>& Coordinates)
{
	for (FHexCoordinates Coordinate : Coordinates)
	{
		AddCreationTileCoordinates(Coordinate);
	}
}


TObjectPtr<AUnitBase> ATeam_PlayerControlled::SpawnUnit(TSubclassOf<AUnitBase> UnitType)
{
	//if(iSpawnPoints <= 0) return nullptr;
	if(Units.Num() >= MaxUnitsSpawned ) return nullptr;

	iSpawnPoints--;
	
	const TObjectPtr<AUnitBase> Unit = Super::SpawnUnit(UnitType);

	// Place unit on creation tile
	const TArray<UHexTile*>& NodeTiles = GameManager->GetGameBoard()->NodeTiles;

	// Check if the index is within valid bounds
	const int32 LastIndex = Units.Num() - 1;
	if ( LastIndex >= 0 && LastIndex < NodeTiles.Num()) {
		// Check if the cast was successful and the pointer is not null
		const TObjectPtr<UHexTile_Creation> CreationTile =
			Cast<UHexTile_Creation>(
				GameBoardUtils::FindNodeByHexCoordinates(CreationTileCoordinates[Units.Num() - 1], NodeTiles));
		if ( CreationTile ) {
			// Place the unit on the tile
			AGameBoard::PlaceUnitOnTile(Unit, CreationTile);
		} else {
			UE_LOG( LogTemp, Warning, TEXT( "ATeam::SpawnUnit: Failed to cast NodeTiles[%d] to UHexTile_Creation" ), LastIndex );
		}
	} else {
		UE_LOG( LogTemp, Warning, TEXT( "ATeam::SpawnUnit: Index %d is out of bounds" ), LastIndex );
	}
	
	return Unit;
}

AUnitBase* ATeam_PlayerControlled::SpawnUnit(int32 UnitTier)
{
	return SpawnUnit(UnitTypes[UnitTier]);
}

TArray<TObjectPtr<AUnitBase>> ATeam_PlayerControlled::SpawnStartUnits()
{
	TArray<TObjectPtr<AUnitBase>> SpawnedUnits;
	for (int32 i = 0; i < iUnitStartAmount; i++)
	{
		SpawnedUnits.Add(SpawnUnit(0));
	}
	return SpawnedUnits;
}

void ATeam_PlayerControlled::OnTurnChanged()
{
	for (TObjectPtr<AUnitBase> Unit : Units)
	{
		Unit->ResetUnit();
	}
	iSpawnPoints = iMaxSpawnPoints;
}
