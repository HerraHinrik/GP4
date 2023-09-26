

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
	moveLink = startTile->GetLinkTo(tile);
	if (!moveLink)
		return;
	Action_TimerDelegate.BindUFunction(this, FName("ExecuteAction"));
	GetWorld()->GetTimerManager().SetTimerForNextTick(Action_TimerDelegate);

	// Set up the timer for testing
	ActionTime = 1.0f;

}

void UAttackAction::ExecuteAction()
{
	FVector TargetLocation;
	FVector StartLocation = Action_Unit->GetActorLocation();
	float Alpha = 0.0f;
	// Tick timer
	ActionTimer += GetWorld()->GetDeltaSeconds();
	// If targetenemy isnt dead
	if(targetEnemy && targetEnemy->IsUnitAlive())
	{
		if ( ActionTimer < ActionTime )
		{
			// Move to enemy and do damage
			if(!bDoneDamage)
			{
				if ( ActionTimer < ActionTime * 0.25f )
				{
					TargetLocation = moveLink->GetTarget()->GetWorldLocation();
					StartLocation += FVector(0.0f, 0.0f, Action_Unit->fHeightOffset);
					Alpha = ActionTimer / (ActionTime * 0.25f);
				}else
				{
					targetEnemy->ReceiveDamage(Action_Unit->iAttackDamage);
					if (targetEnemy->bRetaliateWhenAttacked)
					{
						Action_Unit->ReceiveDamage(targetEnemy->iAttackDamage);
					}
					bDoneDamage = true;
				}
			}
			else // Already done damage -> Move back to start tile
			{
				TargetLocation = moveLink->GetSource()->GetWorldLocation();
				StartLocation += FVector(0.0f, 0.0f, Action_Unit->fHeightOffset / 5.0f);
				Alpha = (ActionTimer - ActionTime * 0.25f) / (ActionTime * 0.75f);
			}
			TargetLocation += FVector(0.0f, 0.0f, Action_Unit->fHeightOffset);
			LerpMoveToTarget(StartLocation, TargetLocation, Alpha);
		}
		else
		{
			ActionTimer = 0.0f;
			EndAction();
			return;
		}
	}else if(targetEnemy && !targetEnemy->IsUnitAlive()) // if targetenemy is dead
	{
		// Move to their tile
		if ( ActionTimer < ActionTime )
		{
			TargetLocation = moveLink->GetTarget()->GetWorldLocation();
			Alpha = ActionTimer / ActionTime;
			LerpMoveToTarget(Action_Unit->GetActorLocation(), TargetLocation + FVector(0.0f, 0.0f, Action_Unit->fHeightOffset), Alpha);
		}
		else
		{
			EndAction();
			return;
		}
	}

	// Schedule next tick
	GetWorld()->GetTimerManager().SetTimerForNextTick(Action_TimerDelegate);
}

void UAttackAction::EndAction()
{
	//reset cached values
	moveLink = nullptr;
	targetEnemy = nullptr;
	bDoneDamage = false;
	//call parent
	Super::EndAction();
}

void UAttackAction::LerpMoveToTarget(const FVector& CurrentLocation,const FVector& TargetLocation, float Alpha) const
{
	// Clamp alpha
	Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);
	const FVector NewLocation = FMath::Lerp(CurrentLocation, TargetLocation, Alpha);
	Action_Unit->SetActorLocation(NewLocation);
}
