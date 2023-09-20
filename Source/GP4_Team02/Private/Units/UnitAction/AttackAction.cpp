

#include "Units/UnitAction/AttackAction.h"

#include "GameBoard/GameBoardUtils.h"
#include "GameBoard/GameBoard.h"
#include "Units/UnitBase.h"
#include "GameBoard/Tiles/HexTile.h"
#include "GameBoard/Link.h"


void UAttackAction::StartAction(UTileBase* tile, AUnitBase* unit)
{
	//null checks
	if (!tile || !unit)
		return;

	//other checks
	if (!bCanPerformAction || !tile->GetOccupyingUnit() || !unit->CanAttack())
		return;



	TObjectPtr<UTileBase> startTile =  unit->GetCurrentTile();
	if (!startTile)
		return;

	//check if target is in range
	TArray<TObjectPtr<UTileBase>> tiles;
	GameBoardUtils::FindNodesWithinRadius(startTile, unit->iAttackRange, tiles);
	if (!tiles.Contains(tile))
		return;
	
	//attack the enemy
	targetEnemy  = tile->GetOccupyingUnit();
	if (!targetEnemy) { return; }
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Attack!!");

	//cache values
	UUnitAction::StartAction(tile, unit);

	targetEnemy->ReceiveDamage(Action_Unit->iAttackDamage);
	if (targetEnemy->bRetaliateWhenAttacked)
	{
		Action_Unit->ReceiveDamage(targetEnemy->iAttackDamage);
	}
	
	//if the enemy is dead, move into his tile
	if (Action_Unit->IsUnitAlive() && !targetEnemy->IsUnitAlive())
	{
		moveLink = startTile->GetLinkTo(tile);
		if (moveLink)
		{
			Action_TimerDelegate.BindUFunction(this, FName("ExecuteAction"));
			GetWorld()->GetTimerManager().SetTimerForNextTick(Action_TimerDelegate);
		}
	}
	else
	{
		EndAction();
	}
	
}

void UAttackAction::ExecuteAction()
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

void UAttackAction::EndAction()
{
	//move unit, set new current tile
	if (Action_Unit && Action_Tile && moveLink)
	{
		AGameBoard::PlaceUnitOnTile( Action_Unit, Action_Tile );
	}
	
	//reset cached values
	moveLink = nullptr;
	Super::EndAction();
}
