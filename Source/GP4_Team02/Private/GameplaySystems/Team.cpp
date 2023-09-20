
#include "GameplaySystems/Team.h"
#include "GameBoard/GameBoard.h"
#include "GameBoard/GameBoardUtils.h"
#include "GameBoard/Tiles/HexTile_Creation.h"
#include "GameplaySystems/TWS_GameManager.h"
#include "Units/UnitBase.h"

void ATeam::BeginPlay()
{
	Super::BeginPlay();
	GameManager = GetWorld()->GetSubsystem<UTWS_GameManager>();
	GameManager->OnTurnChanged.AddDynamic( this, &ATeam::OnTurnChanged );

	// Set spawn points to the start units amount
	iSpawnPoints = iUnitStartAmount;
}

TObjectPtr<AUnitBase> ATeam::SpawnUnit(TSubclassOf<AUnitBase> UnitType)
{
	//if(iSpawnPoints <= 0) return nullptr;
	if(Units.Num() >= 4 ) return nullptr;

	iSpawnPoints--;
	
	const TObjectPtr<AUnitBase> Unit = GetWorld()->SpawnActor<AUnitBase>(UnitType);
	Unit->SetTeam(this);
	Unit->OnUnitDeath.AddDynamic(this, &ATeam::RemoveDeadUnits);
	Units.Add(Unit);
	OnUnitsChanged.Broadcast();

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

AUnitBase* ATeam::SpawnUnit(int32 UnitTier)
{
	if(UnitTypes.Num() == 0) return nullptr;
	// Clamp the tier to the number of unit types
	UnitTier = FMath::Clamp(UnitTier, 0, UnitTypes.Num() - 1);
	return SpawnUnit(UnitTypes[UnitTier]);	
}

void ATeam::AddCreationTile(const TObjectPtr<UHexTile_Creation> Tile)
{
	if(!Tile) return;
	if(!CreationTiles.Contains(Tile))
		CreationTiles.Add(Tile);
	
	const FHexCoordinates Coordinates = Tile->GetHexCoordinates();
	if(!CreationTileCoordinates.Contains(Coordinates))
		AddCreationTileCoordinates(Coordinates);
}

void ATeam::AppendCreationTiles(const TArray<TObjectPtr<UHexTile_Creation>>& Tiles)
{
	for (TObjectPtr<UHexTile_Creation> Tile : Tiles)
	{
		AddCreationTile(Tile);
	}
}

void ATeam::AddCreationTileCoordinates(const FHexCoordinates& Coordinates)
{
	if(!CreationTileCoordinates.Contains(Coordinates))
		CreationTileCoordinates.Add(Coordinates);

	if(!GameManager) return;
	const TArray<UHexTile*> Tiles = GameManager->GetGameBoard()->NodeTiles;
	if( Tiles.Num() == 0 ) return;
	if(const TObjectPtr<UHexTile_Creation> CreationTile =  Cast<UHexTile_Creation>(GameBoardUtils::FindNodeByHexCoordinates(Coordinates, Tiles)))
		AddCreationTile(CreationTile);

}

void ATeam::AppendCreationTileCoordinates(const TArray<FHexCoordinates>& Coordinates)
{
	for (FHexCoordinates Coordinate : Coordinates)
	{
		AddCreationTileCoordinates(Coordinate);
	}
}

void ATeam::RemoveDeadUnits()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Death");
	TArray<TObjectPtr<AUnitBase>> LivingUnits;
	for (TObjectPtr<AUnitBase> unit : Units)
	{
		if (!unit)
			continue;
		
		if(unit->IsUnitAlive())
		{
			LivingUnits.Add(unit);
		}
		else
		{
			UnitPool.Add(unit);
		}
	}
	
	if (LivingUnits.IsEmpty())
	{
		AllUnitsDead();
		Units.Empty();
	}
	else
	{
		Units = LivingUnits;
	}
	OnUnitsChanged.Broadcast();
}

void ATeam::AllUnitsDead()
{
	GameManager->CheckForWin();
}

void ATeam::OnTurnChanged()
{
	for (TObjectPtr<AUnitBase> Unit : Units)
	{
		Unit->ResetUnit();
	}
	iSpawnPoints = iMaxSpawnPoints;

	GEngine->AddOnScreenDebugMessage( -1, 10.f, FColor::Red, "Turn changed");
}