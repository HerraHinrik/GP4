
#include "Units/UnitAction/MoveAction.h"

#include "GameBoard/GameBoard.h"
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
	UTileBase* startTile =  unit->GetCurrentTile();
	if (!startTile)
		return;

	moveLink = startTile->GetLinkTo(tile);
	if (!moveLink)
		return;

	if (moveLink)
	{
		Action_TimerDelegate.BindUFunction(this, FName("ExecuteAction"));
		GetWorld()->GetTimerManager().SetTimerForNextTick(Action_TimerDelegate);
		//cache values
		UUnitAction::StartAction(tile, unit);
		
	}
	
}

void UMoveAction::ExecuteAction()
{
	if ( ActionTimer < ActionTime )
	{
		// Lerp world position towards target tile over the action time
		const FVector NewLocation = FMath::Lerp(Action_Unit->GetActorLocation(), moveLink->GetTarget()->GetWorldLocation() + FVector(0,0,Action_Unit->fHeightOffset), ActionTimer / ActionTime);
		Action_Unit->SetActorLocation(NewLocation);
		ActionTimer += GetWorld()->GetDeltaSeconds();
		GetWorld()->GetTimerManager().SetTimerForNextTick(Action_TimerDelegate);
	}else
	{
		EndAction();
	}
}

void UMoveAction::EndAction()
{
	//move unit, set new current tile
	if (Action_Unit && Action_Tile && moveLink)
	{
		AGameBoard::PlaceUnitOnTile( Action_Unit, Action_Tile );
	}

	//reset cached values
	moveLink = nullptr;
	UUnitAction::EndAction();
}
