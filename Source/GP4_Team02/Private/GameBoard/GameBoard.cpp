
#include "GameBoard/GameBoard.h"
#include "GameBoard/Tiles/HexTile.h"
#include "GameBoard/GameBoardUtils.h"
#include "GameBoard/HighlightSystem.h"
#include "GameBoard/Tiles/HexTile_Creation.h"
#include "GameBoard/Tiles/Tile_ClaimableHexTile.h"
#include "GameBoard/Tiles/HexTile_Influence.h"
#include "GameplaySystems/Team_PlayerControlled.h"
#include "GameplaySystems/TWS_GameManager.h"
#include "Units/UnitBase.h"

#if !UE_BUILD_SHIPPING
#include "Debug/GameBoardDebug.h"
#endif // !UE_BUILD_SHIPPING

AGameBoard::AGameBoard()
{
	HighlightSystem = CreateDefaultSubobject<UHighlightSystem>(TEXT("HighlightSystem"));
}

void AGameBoard::BeginPlay()
{
	Super::BeginPlay();
	const TObjectPtr<UTWS_GameManager> GameManager = GetWorld()->GetSubsystem<UTWS_GameManager>();
	GameManager->SetGameBoard(this);
	GameManager->InitializeGame( TeamTypes );
	GameManager->SetTurnMaxTime(TurnTime);
	HighlightSystem->Init(GameManager);
}

void AGameBoard::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	// Clear board before constructing
	ClearGameBoard();
	
	// Construct tile grid in dev build -> Might throw errors out so skip in shiping build
#if !UE_BUILD_SHIPPING
	ConstructTileGrid(0,0);
#endif // !UE_BUILD_SHIPPING
	
}

void AGameBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameBoard::ClearGameBoard()
{
	// Clear board before constructing
	for (UHexTile* HexTile : NodeTiles)
	{
		if (HexTile != nullptr)
			HexTile->DestroyTile();
	}
	NodeTiles.Empty();
}

void AGameBoard::ConstructTileGrid(int32 PlayerControlledTeamAmount, int32 AIControlledTeamAmount)
{
	// Decide which type of grid to create ( Hexagonal or Square )
	ConstructHexagonalTileGrid(PlayerControlledTeamAmount, AIControlledTeamAmount);

#if !UE_BUILD_SHIPPING // Debug stuff if we're in editor
	if(bShowLinks)
	{
		GameBoardDebug::VisualizeLinksBetweenTiles(this, GetWorld());
	}
	if(bShowGridCoordinates)
	{
		GameBoardDebug::AddDebugCoordinatesForGameBoard(this, GetWorld());
	}
#endif // !UE_BUILD_SHIPPING
}

void AGameBoard::ConstructHexagonalTileGrid(int32 PlayerControlledTeamAmount, int32 AIControlledTeamAmount)
{
	// Clear board before constructing
	ClearGameBoard();
	
	if(StandardNodeTypes.IsEmpty() || StandardNodeTypes.Contains(nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("StandardNodeTypes is null or empty in AGameBoard::ConstructHexagonalTileGrid()"));
		return;
	}

	TMap<FHexCoordinates, TSubclassOf<UTileBase>> TilesMap;

    for (int q = -GridWidth; q <= GridWidth; q++) {
        int R1 = std::max(-GridWidth, -q - GridWidth);
        int R2 = std::min(GridWidth, -q + GridWidth);
        for (int r = R1; r <= R2; r++) {
            int s = -q - r;

        	// Check if the current tile should be created (based on GridMaxHeight)
        	if(FMath::Abs(r) > GridMaxHeight) continue;

        	// Create a node tile
            FHexCoordinates Hex(q, r, s);
            	
           	// Set the tile type as a random standard tile type
			TSubclassOf<UTileBase> TileType = StandardNodeTypes[FMath::RandRange(0, StandardNodeTypes.Num() - 1)];

           	// Replace the tile type if the tile should be a claimable tile
           	if(ClaimableTileType != nullptr && ClaimableTileCoordinates.Contains(Hex))
           	{
           		TileType = ClaimableTileType;
           	}
        	// Replace with influence if the tile should be an influence tile
		   	else if(InfluenceTileType != nullptr && InfluenceTileCoordinates.Contains(Hex))
		   	{
		   		TileType = InfluenceTileType;
		   	}
            	
           	TilesMap.Add(Hex, TileType);
            
        }
    }

	// Create the creation tiles
	constexpr EHexDirection CornerDirectionOrder[6] = {
    	EHexDirection::Left,
		EHexDirection::Right,
		EHexDirection::TopRight,
		EHexDirection::BottomRight,
		EHexDirection::BottomLeft,
		EHexDirection::TopLeft,
		};

	TArray<FHexCoordinates> TilesCoordinates;
	TilesMap.GetKeys(TilesCoordinates);
	
	
	const TObjectPtr<UTWS_GameManager> GameManager = GetWorld()->GetSubsystem<UTWS_GameManager>();
	if(!GameManager || GameManager->PlayerControlledTeamAmount <= 0)
		return;
	for (int32 i = 0; i < GameManager->PlayerControlledTeamAmount; i++)
	{
		// Get corner coordinate based on index and direction set above
		const FHexCoordinates CornerHex = GameBoardUtils::GetCornerHexCoordinate( CornerDirectionOrder[i], TilesCoordinates );

		if(!TilesMap.Contains(CornerHex))
			continue;
		
		// Find all tiles adjacent to the corner tile
		TArray<FHexCoordinates> AdjacentTiles = GameBoardUtils::GetAdjacentHexCoordinates( CornerHex, TilesCoordinates );
		// Replace the tile type of the corner tile and its adjacent tiles with the creation tile type
		TObjectPtr<ATeam_PlayerControlled> PlayerTeam = Cast<ATeam_PlayerControlled>(GameManager->GetTeamArray()[i]);
		if(!PlayerTeam) continue;
		
		TSubclassOf<UHexTile_Creation> CreationTileType = PlayerTeam->GetCreationTileType();
		if(CreationTileType == nullptr) continue;
		
		TilesMap[CornerHex] = CreationTileType;
		PlayerTeam->AddCreationTileCoordinates( CornerHex );
		PlayerTeam->AppendCreationTileCoordinates( AdjacentTiles );

		for (FHexCoordinates AdjacentTile : AdjacentTiles)
		{
			TilesMap[AdjacentTile] = CreationTileType;
		}
	}

	// Loop through the Map and create the tiles
	for (TTuple<FHexCoordinates, TSubclassOf<UTileBase>> Map : TilesMap)
	{
		CreateTileComponent(Map.Value, Map.Key);
	}

	// Loop through the node tiles and connect them to their neighbors
	for (UHexTile* HexTile : NodeTiles)
	{
		ConnectNodeToNeighbors(HexTile);
	}
	
}

TObjectPtr<UTileBase> AGameBoard::CreateTileComponent(TSubclassOf<UTileBase> TileType, const FHexCoordinates& HexCoordinates)
{
	if(TileType == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("TileType is null in AGameBoard::CreateTileComponent()"));
		return nullptr;
	}
	
	const TObjectPtr<UTileBase> NewNodeTile = NewObject<UHexTile>(this, TileType);
	NewNodeTile->RegisterComponent();
	NewNodeTile->AttachToComponent( RootComponent, FAttachmentTransformRules::KeepRelativeTransform );
	AddOwnedComponent(NewNodeTile);
	NewNodeTile->SetGameBoardParent(this);
	
	if(TObjectPtr<UHexTile> HexTile = Cast<UHexTile>(NewNodeTile))
	{
		// Set the hex coordinates of the node tile
		HexTile->SetHexCoordinates(HexCoordinates);
		// Add the node tile to the array of node tiles
		NodeTiles.Add(HexTile);
		// Convert hexagonal coordinates to world coordinates
		const FVector WorldPosition = GameBoardUtils::HexToWorld(HexCoordinates, HexTile->GetWidth(), HexTile->GetHeight(), TileSpacing);
		// Set the node tile's location
		HexTile->SetWorldLocation(WorldPosition);

	}

	return NewNodeTile;
}

void AGameBoard::ConnectNodeToNeighbors(UHexTile* Node) const
{

	// Get the hex coordinates of the current node
	const FHexCoordinates CurrentNodeHex = Node->GetHexCoordinates();

	for (int32 DirectionIndex = 0; DirectionIndex < 6; DirectionIndex++) {

		// Calculate the hex coordinates of the neighboring node
		FHexCoordinates NeighborHex = CurrentNodeHex + FHexDirectionVectors[DirectionIndex];

		// Find the neighboring node based on its hex coordinates
		UHexTile* NeighborNode = GameBoardUtils::FindNodeByHexCoordinates(NeighborHex, NodeTiles);

		if (NeighborNode) {
			// Create a link between the current node and its neighbor
			Node->AddLink(UTileBase::CreateLink(Node, NeighborNode));
		}
	}
	
}

bool AGameBoard::PlaceUnitOnTile(AUnitBase* Unit, UTileBase* Tile)
{
	// Null Check
	if( !Unit || !Tile )
	{
		UE_LOG( LogTemp, Warning, TEXT( "Unit or Tile is null" ) );
		return false;
	}
	// Check if the tile is occupied
	if( Tile->GetOccupyingUnit() )
	{
		UE_LOG( LogTemp, Warning, TEXT( "Tile is occupied" ) );
		return false;
	}
	
	// If all checks pass, place the unit on the tile and return true
	FVector TargetLocation = Tile->GetWorldLocation();
	TargetLocation.Z += Unit->fHeightOffset;
	Unit->SetActorLocation(TargetLocation);
	Unit->SetCurrentTile(Tile);
	Tile->SetOccupyingUnit(Unit);
	return true;
}







