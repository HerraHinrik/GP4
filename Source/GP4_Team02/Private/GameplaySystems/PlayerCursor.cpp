

#include "GameplaySystems/PlayerCursor.h"
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
	}
	return bGotNewUnit;
}

bool APlayerCursor::SelectTile(TObjectPtr<UTileBase> Tile)
{
		if (Tile)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Select Tile");

			if(SelectedTile)
				SelectedTile->SelectTile(false);
			
			SelectedTile = Tile;
			Tile->SelectTile(true);
			if (SelectedUnit && SelectedTile)
			{
				if (!SelectedTile->GetOccupyingUnit())
				{
					SelectedAction = SelectedUnit->GetMoveAction();
				}
				else 
				{
					if(SelectedTile->GetOccupyingUnit()->GetTeam() != myTeam)
						SelectedAction = SelectedUnit->GetOffensiveAction();
					else
					{
						SelectedAction = SelectedUnit->GetSupportiveAction();
					}
				}
				
				if (SelectedAction && SelectedUnit->CanAffordAction(SelectedAction->iActionCost))
				{
					PerformAction();
				}
			}
			// Debug Sphere on tile
			//UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Tile->GetWorldLocation() + FVector(0,0,100.0f), 10.0f, 12, FColor::Red, 10.0f, 5.0f
		}
	if (SelectedTile) { return true; }
	
	return false;
}

bool APlayerCursor::SelectAction(TObjectPtr<UUnitAction>)
{
	if (SelectedAction) { return true; }

	return false;
}

void APlayerCursor::CheckIfMyTurn()
{
	Super::CheckIfMyTurn();
	GEngine->AddOnScreenDebugMessage( -1, 1.f, FColor::Red, "CheckIfMyTurn" );	
	DeselectTarget();
}

void APlayerCursor::Test()
{
	CheckIfMyTurn();
}

void APlayerCursor::PerformAction()
{
	if (SelectedAction && SelectedUnit)
	{
		SelectedAction->StartAction(SelectedTile, SelectedUnit);
		SelectedAction = nullptr;
	}
}

void APlayerCursor::SelectTarget()
{
	if (!myTeam && Controller)
		myTeam = Controller->GetTeam();

	
	if (GameManager->GetCurrentTeam() != GetTeam() || !GameManager->GetTurnActive())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Not Your Turn");
		return;	
	}

	if (SelectUnit(HoveredTile)) { return; }

	if (SelectTile(HoveredTile)) { return; }

}

void APlayerCursor::DeselectTarget()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Deselect");
	
	SelectedUnit = nullptr;

	if (SelectedTile)
	{
		SelectedTile->SelectTile(false);
		SelectedTile = nullptr;
	}
	

	SelectedAction = nullptr;
}

#pragma endregion

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
					// If we had a hovered tile before
					if (HoveredTile)
					{
						HoveredTile->HoverTile(false);
					}
					
					HoveredTile = Tile;
					HoveredTile->HoverTile(true);
					break;
				}
			}
		}
	}
}
