#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TileBase.generated.h"

class AGameBoard;
class AUnitBase;
class ULink;

// Delegates for the tile
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileHovered, bool, bHovered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileSelected, bool, bSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNeighbour, bool, bIsNeighbour);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP4_TEAM02_API UTileBase : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// Constructor
	UTileBase();

	// Link functions
	static ULink* CreateLink(TObjectPtr<UTileBase> Source, TObjectPtr<UTileBase> Target);
	virtual void AddLink(ULink* link);
	void AddLink(TObjectPtr<UTileBase> source, TObjectPtr<UTileBase> target);
	void RemoveLink(ULink* link);
	void RemoveLink(TObjectPtr<UTileBase> target);
	TArray<ULink*> GetLinks() { return Links; }
	ULink* GetLinkTo(TObjectPtr<UTileBase> target);

	// Set/Get Unit
	virtual void SetOccupyingUnit(AUnitBase* unit) { Unit = unit; }
	UFUNCTION( BlueprintCallable, Category = "Tile" )
	AUnitBase* GetOccupyingUnit() const { return Unit; }
	// Remove Unit
	void RemoveUnit() { Unit = nullptr; }

	// Returns the World Location of the tile
	UFUNCTION( BlueprintCallable, Category = "Tile" )
	virtual FVector GetWorldLocation() { return GetComponentLocation();}
	// Returns the World Rotation of the tile
	virtual FRotator GetWorldRotation() { return GetComponentRotation();}

	// Return Height 
	virtual float GetHeight() const { return 0; }

	// Return Width 
	virtual float GetWidth() const { return 0; }

	// Get/Set coordinates
	virtual FVector GetCoordinates() const { return FVector(0,0,0); }
	virtual void SetCoordinates(FVector Coordinates) {  }

	// DestroyNode
	virtual void DestroyTile();

	// Select tile
	virtual void SelectTile(bool bSelected);

	// Hover tile
	void HoverTile(bool bHovered);
	
	// Set/Get Parent GameBoard
	virtual void SetGameBoardParent(const TObjectPtr<AGameBoard> GameBoard) { ParentGameBoard = GameBoard; }
	TObjectPtr<AGameBoard> GetGameBoardParent() const { return ParentGameBoard; }

	// Reset A* variables
	void ResetAStarVariables() { ParentNode = nullptr; GCost = 0.0f; HCost = 0.0f; FCost = 0.0f; }

	

	// Virtual Destructor
	//virtual ~UTileBase() {}

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Tile")
	FOnTileHovered OnTileHovered;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Tile")
	FOnTileSelected OnTileSelected;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Tile")
	FOnNeighbour OnNeighbour;

	// A* variables
	// The parent node of this node in the path
	UPROPERTY()
	TObjectPtr<UTileBase> ParentNode = nullptr;
	// The cost of the path from the start node to this node
	float GCost = 0.0f;
	// The estimated cost of the path from this node to the end node
	float HCost = 0.0f;
	// The total cost of the path from the start node to the end node through this node
	float FCost = 0.0f;
	

protected:
	// Links to other tiles
	UPROPERTY()
	TArray<ULink*> Links = TArray<ULink*>();
	// Tile Width
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	float TileWidth;
	// Occupying Unit
	TObjectPtr<AUnitBase> Unit = nullptr;
	// Parent GameBoard
	TObjectPtr<AGameBoard> ParentGameBoard;

};
