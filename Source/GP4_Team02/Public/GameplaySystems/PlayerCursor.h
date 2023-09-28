
#pragma once

#include "CoreMinimal.h"
#include "Team.h"
#include "TeamPawn.h"
#include "GameBoard/Tiles/TileBase.h"
#include "PlayerCursor.generated.h"

class ULink;
class APlayerInputController;
class AUnitBase;
class UTileBase;
class UUnitAction;
class UTWS_GameManager;

UCLASS()
class GP4_TEAM02_API APlayerCursor : public ATeamPawn
{
	GENERATED_BODY()

public:
	APlayerCursor();

private:
	TObjectPtr<APlayerInputController> Controller;
	TObjectPtr<UTileBase> HoveredTile = nullptr;
	TArray<TObjectPtr<ULink>> PathLinks;
	// TObjectPtr<ATeam> myTeam;

	void UpdateHoveredTile();

#pragma region SelectAction
	TObjectPtr<AUnitBase> SelectedUnit  = nullptr;
	TObjectPtr<UTileBase> SelectedTile = nullptr;
	TObjectPtr<UUnitAction> SelectedAction = nullptr;
	
	void PerformAction();

	bool SelectUnit(TObjectPtr<UTileBase> Tile);
	bool SelectTile(TObjectPtr<UTileBase> Tile);
	bool SelectAction(TObjectPtr<UUnitAction>);
#pragma endregion

protected:
	virtual void BeginPlay() override;

	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double fXMax = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double fYMax = 700.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CursorSpeed_Overall = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CursorSpeed_X = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CursorSpeed_Y = 10;

	UFUNCTION(BlueprintImplementableEvent)
	void OpenActionsMenu();


	TObjectPtr<ATeam> GetTeam() const { return myTeam; }
	void SetTeam(TObjectPtr<ATeam> team) { myTeam = team; team->SetPlayerCursor(this); }
	
	TObjectPtr<AUnitBase> GetSelectedUnit() const { return SelectedUnit; }
	TObjectPtr<UTileBase> GetSelectedTile() const { return SelectedTile; }
	void SetSelectedTile(const TObjectPtr<UTileBase> Tile)
	{
		if(SelectedTile)
			SelectedTile->SelectTile(false);
		SelectedTile = Tile;
		SelectedTile->SelectTile(true);
	}

	virtual void CheckIfMyTurn() override;
	
	void SelectTarget();

	void DeselectTarget();

	void ClaimTile();

	void EndTurn();

	void DestroySelectedUnit();
	
	void MovePlayerCursor(FVector2D inputVector);
};
