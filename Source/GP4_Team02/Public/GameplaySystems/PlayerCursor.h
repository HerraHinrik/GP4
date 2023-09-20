
#pragma once

#include "CoreMinimal.h"
#include "TeamPawn.h"
#include "PlayerCursor.generated.h"

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
	virtual void CheckIfMyTurn() override;
	void Test();
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
	void SetTeam(TObjectPtr<ATeam> team) { myTeam = team; }
	
	TObjectPtr<AUnitBase> GetSelectedUnit() { return SelectedUnit; }
	TObjectPtr<UTileBase> GetSelectedTile() { return SelectedTile; }
	void SelectTarget();

	void DeselectTarget();
	
	void MovePlayerCursor(FVector2D inputVector);
};
