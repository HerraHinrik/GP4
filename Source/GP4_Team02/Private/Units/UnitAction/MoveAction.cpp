
#include "Units/UnitAction/MoveAction.h"

#include "GameBoard/GameBoard.h"
#include "GameBoard/GameBoardUtils.h"
#include "Units/UnitBase.h"
#include "GameBoard/Tiles/HexTile.h"
#include "GameBoard/Link.h"

void UMoveAction::StartAction(UTileBase* tile, AUnitBase* unit)
{
	//null checks
	if (!tile || !unit)
		return;

	//other checks
	if (!bCanPerformAction || tile->GetOccupyingUnit())
		return;
	
	//check link from current tile to target tile
	UTileBase* StartTile =  unit->GetCurrentTile();
	if (!StartTile)
		return;

	Path = GameBoardUtils::FindPathInHexGrid(StartTile, tile);
	if (Path.Num() == 0)
		return;

	Action_TimerDelegate.BindUFunction(this, FName("ExecuteAction"));
	GetWorld()->GetTimerManager().SetTimerForNextTick(Action_TimerDelegate);
	//cache values
	UUnitAction::StartAction(tile, unit);
	
}

void UMoveAction::ExecuteAction()
{
	// Get new movelink or end action if we have no action movelink
	if( !MoveLink )
	{
		if( Path.Num() > 0 && iTilesMoved * iActionCost < Action_Unit->GetRemainingActionPoints())
		{
			MoveLink = Path[0];
			Path.RemoveAt(0);
			GetWorld()->GetTimerManager().SetTimerForNextTick(Action_TimerDelegate);
		}else
		{
			EndAction();
			return;
		}
	}else
	{
		// Move unit towards links target tile
		if ( ActionTimer < ActionTime )
		{
			// Lerp world position towards target tile over the action time
			const FVector NewLocation = FMath::Lerp(Action_Unit->GetActorLocation(), MoveLink->GetTarget()->GetWorldLocation() + FVector(0,0,Action_Unit->fHeightOffset), ActionTimer / ActionTime);
			Action_Unit->SetActorLocation(NewLocation);
			ActionTimer += GetWorld()->GetDeltaSeconds();
		}
		else // Reset timer and place unit on target tile
		{
			AGameBoard::PlaceUnitOnTile( Action_Unit, MoveLink->GetTarget() );
			MoveLink = nullptr;
			ActionTimer = 0.0f;
			iTilesMoved++;
		}
		GetWorld()->GetTimerManager().SetTimerForNextTick(Action_TimerDelegate);
	}
}

void UMoveAction::EndAction()
{

	const int32 iStoredActionCost = iActionCost;
	iActionCost *= iTilesMoved;
	//reset cached values
	MoveLink = nullptr;
	Path.Empty();
	UUnitAction::EndAction();
	iActionCost = iStoredActionCost;
	iTilesMoved = 0;
}
