

#include "GameplaySystems/PlayerCursor.h"

#include "GameBoard/GameBoard.h"
#include "GameBoard/HighlightSystem.h"
#include "GameplaySystems/PlayerInputController.h"
#include "GameplaySystems/TWS_GameManager.h"
#include "Units/UnitBase.h"
#include "Units/UnitAction/UnitAction_Supportive.h"


APlayerCursor::APlayerCursor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerCursor::BeginPlay()
{
	Super::BeginPlay();
	Controller = Cast<APlayerInputController>(GetController());
}

void APlayerCursor::CheckIfMyTurn()
{
	Super::CheckIfMyTurn();
	DeselectTarget();
}

#pragma region SelectFunctions

bool APlayerCursor::SelectUnit(TObjectPtr<UTileBase> Tile)
{
	bool bGotNewUnit = false;

	TObjectPtr<AUnitBase> unitOnTile;
	if (Tile)
		unitOnTile = Tile->GetOccupyingUnit();

	if (unitOnTile && unitOnTile->GetTeam() == myTeam)
	{
		if (SelectedTile)
		{
			SelectedTile->SelectTile(false);
		}
		SelectedTile = Tile;
		SelectedTile->SelectTile(true);
		SelectedUnit = unitOnTile;
		bGotNewUnit = true;
		HoveredTile->GetGameBoardParent()->HighlightSystem->SetSelectedTile(HoveredTile);

	}
	return bGotNewUnit;
}

bool APlayerCursor::SelectTile(TObjectPtr<UTileBase> Tile)
{
		if (Tile && bIsMyTurn)
		{

			// If we have a previous selected tile, deselect it
			SetSelectedTile(Tile);

			// If we have a selected unit, check which action is appropriate and see if we can perform the action
			if (SelectedUnit && SelectedTile)
			{
				if (!SelectedTile->GetOccupyingUnit()) // If the tile is empty
				{
					SelectedAction = SelectedUnit->GetMoveAction();
				}
				else 
				{
					if(SelectedTile->GetOccupyingUnit()->GetTeam() != myTeam)	// If the tile is occupied by an enemy
					{
						SelectedAction = SelectedUnit->GetOffensiveAction();
					}
					else														// If the tile is occupied by an ally
					{
						SelectedAction = SelectedUnit->GetSupportiveAction();
					}
				}
				
				if (SelectedAction && SelectedUnit->CanAffordAction(SelectedAction->iActionCost))
				{
					PerformAction();
				}
			}
		}
	if (SelectedTile) { return true; }
	
	return false;
}

bool APlayerCursor::SelectAction(TObjectPtr<UUnitAction>)
{
	if (SelectedAction) { return true; }

	return false;
}

void APlayerCursor::SelectTarget()
{
	// Make sure myTeam is set
	if (!myTeam && Controller)
		myTeam = Controller->GetTeam();

	// Make sure it's my turn
	if (!bIsMyTurn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Not Your Turn");
		return;	
	}

	
	if (!SelectedUnit || // If we don't have a selected unit
		!SelectedUnit->GetSupportiveAction() || // If the selected unit doesn't have a supportive action
		SelectedUnit->GetRemainingActionPoints() <= 0) // If the selected unit doesn't have any action points left
	{
		if (SelectUnit(HoveredTile)) { return; } // If we can select a new unit, select it and return
	}

	if (SelectTile(HoveredTile)) { return; } // If we can select a new tile, select it and return

}

void APlayerCursor::DeselectTarget()
{
	SelectedUnit = nullptr;
	if (SelectedTile)
	{
		SelectedTile->SelectTile(false);
		SelectedTile = nullptr;
	}
	SelectedAction = nullptr;
}

#pragma endregion

void APlayerCursor::ClaimTile()
{
	if (!bIsMyTurn || !SelectedUnit)
		return;

	if (TObjectPtr<UClaimTileAction> claimAction = SelectedUnit->GetClaimTileAction() )
	{
		if (SelectedUnit->CanAffordAction(claimAction->iActionCost))
		{
			claimAction->StartAction(SelectedUnit->GetCurrentTile(), SelectedUnit);
		}
	}
}

void APlayerCursor::EndTurn()
{
	if (!bIsMyTurn)
		return;

	GameManager->EndTurn();
}

void APlayerCursor::DestroySelectedUnit()
{
	if (!bIsMyTurn || !SelectedUnit)
		return;
	SelectedUnit->ReceiveDamage(666);
	SelectedUnit = nullptr;
}

void APlayerCursor::PerformAction()
{
	if (SelectedAction && SelectedUnit && SelectedTile)
	{
		SelectedAction->StartAction(SelectedTile, SelectedUnit);
		SelectedAction = nullptr;
	}
}

void APlayerCursor::MovePlayerCursor(FVector2D inputVector)
{
	FVector moveVector = GetActorLocation();

	moveVector.X += inputVector.X * CursorSpeed_Overall * CursorSpeed_X;
	moveVector.Y -= inputVector.Y * CursorSpeed_Overall * CursorSpeed_Y;

	moveVector.X = FMath::Clamp(moveVector.X, -fXMax, fXMax);
	moveVector.Y = FMath::Clamp(moveVector.Y, -fYMax, fYMax);
		
	SetActorLocation(moveVector);
	UpdateHoveredTile();
}

void APlayerCursor::UpdateHoveredTile()
{
	//raycast settings
	TArray<FHitResult> HitResults;
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = GetActorLocation() - FVector(0, 0, 500);
	// UKismetSystemLibrary::DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Magenta, 10.0f, 5.0f);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	if(GetWorld()->LineTraceMultiByChannel(HitResults, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
	{
		for (const FHitResult& HitResult : HitResults)
		{
			if (TObjectPtr<UTileBase> Tile = Cast<UTileBase>(HitResult.Component))
			{
				// If the tile is not the same as the hovered tile
				if (Tile != HoveredTile)
				{
					HoveredTile = Tile;
			
					HoveredTile->GetGameBoardParent()->HighlightSystem->SetHoverTile(HoveredTile);
					
					break;
				}

			}
		}
	}
}
