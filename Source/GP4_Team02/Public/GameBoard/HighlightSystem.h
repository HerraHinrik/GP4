#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HighlightSystem.generated.h"

class UTWS_GameManager;
class AUnitBase;
class UTileBase;

UCLASS()
class GP4_TEAM02_API UHighlightSystem  : public UObject
{
	GENERATED_BODY()
public:
	void Init(TObjectPtr<UTWS_GameManager> GameManager);

	void SetSelectedTile(TObjectPtr<UTileBase> Tile);
	void SetHoverTile(TObjectPtr<UTileBase> Tile);

	UFUNCTION()
	void ResetAllHighLights();
	void ResetHoveredHighLight();
	void ResetNeighbourHighLight();

private:
	TArray<TObjectPtr<UTileBase>> Tiles;
	TArray<TObjectPtr<UTileBase>> HoveredTiles;
	TArray<TObjectPtr<UTileBase>> NeighbourTiles;
	TObjectPtr<UTileBase> SelectedTile;
	
};
