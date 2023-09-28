
#include "Units/UnitAction.h"

#include "GameplaySystems/PlayerCursor.h"
#include "GameplaySystems/TWS_GameManager.h"
#include "Units/UnitBase.h"

void UUnitAction::BeginPlay()
{
	Super::BeginPlay();

	GameManager = GetWorld()->GetSubsystem<UTWS_GameManager>();
}

void UUnitAction::StartAction(UTileBase* tile, AUnitBase* unit)
{
	Action_Tile = tile;
	Action_Unit = unit;
	bCanPerformAction = false;
	OnActionStarted.Broadcast(tile);
}

FVector UUnitAction::GetActionTargetPosition()
{
	return Action_Tile->GetWorldLocation();
}

FVector UUnitAction::GetActingUnitPosition()
{
	return Action_Unit->GetActorLocation();
}

void UUnitAction::ExecuteAction()
{
	return;
}

void UUnitAction::EndAction()
{
	// Consume action points
	Action_Unit->ConsumeActionPoints(iActionCost);
	// Select the target tile
	Action_Unit->GetTeam()->GetPlayerCursor()->SetSelectedTile(Action_Unit->GetCurrentTile());

	Action_Tile = nullptr;
	Action_Unit = nullptr;
	bCanPerformAction = true;
	ActionTimer = 0.0f;
	OnActionCompleted.Broadcast();
}
