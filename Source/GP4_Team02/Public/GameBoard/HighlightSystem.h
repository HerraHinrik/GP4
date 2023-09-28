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
	UFUNCTION(BlueprintCallable)
	void HighLightCreationTiles(class ATeam* Team);

	UFUNCTION()
	void ResetAllHighLights();
	UFUNCTION()
	void ResetHoveredHighLight();
	UFUNCTION()
	void ResetNeighbourHighLight();
	UFUNCTION()
	void ResetCreationTilesHighLight();

private:
	TArray<TObjectPtr<UTileBase>> HoveredTiles;
	TArray<TObjectPtr<UTileBase>> NeighbourTiles;
	TArray<TObjectPtr<class UHexTile_Creation>> CreationTiles;
	TObjectPtr<UTileBase> SelectedTile;
	
};
