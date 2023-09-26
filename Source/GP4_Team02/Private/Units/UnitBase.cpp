
#include "Units/UnitBase.h"
#include "GameBoard/Link.h"
#include "GameBoard/Tiles/HexTile.h"
#include "GameBoard/Tiles/HexTile_Creation.h"
#include "GameplaySystems/Team.h"
#include "GameplaySystems/TWS_GameManager.h"
#include "Units/UnitConditions/UnitCondition_Base.h"

AUnitBase::AUnitBase()
{
	PrimaryActorTick.bCanEverTick = true;

	moveAction = CreateDefaultSubobject<UMoveAction>(FName("Move Action"));
	attackAction = CreateDefaultSubobject<UAttackAction>(FName("Attack Action"));
	claimTileAction = CreateDefaultSubobject<UClaimTileAction>(FName("Claim Tile Action"));
	
	UnitActions.Add(moveAction);
	moveAction->OnActionCompleted.AddDynamic(this, &AUnitBase::OnActionCompleted);
	UnitActions.Add(attackAction);
	attackAction->OnActionCompleted.AddDynamic(this, &AUnitBase::OnActionCompleted);
	UnitActions.Add(claimTileAction);
	claimTileAction->OnActionCompleted.AddDynamic(this, &AUnitBase::OnActionCompleted);
}

void AUnitBase::BeginPlay()
{
	Super::BeginPlay();

	iUnitActionPoints = iMaxActionPoints;
	iCurrentHealth = iMaxHealth;
	
}

void AUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUnitBase::OnActionCompleted()
{
	if (iUnitActionPoints <= 0)
	{
		myTeam->CheckEndTurn();
	}
}

void AUnitBase::GetAdjacentTiles(TArray<UTileBase*>& tileArray)
{
	TArray<TObjectPtr<UTileBase>> outArray;
	if (!CurrentTile)
		return;
	
	TArray<ULink*> links = CurrentTile->GetLinks();
	if (links.IsEmpty() || links.Num() <= 0)
	{
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Emerald, "No Adjacent Links");
		return;
	}
	
	for(int i = 0; i < links.Num(); i++)
	{
		TObjectPtr<UTileBase> tile = Cast<UTileBase>(links[i]->GetTarget());
		if (tile)
		{
			outArray.Add(tile);
		}
	}
	
	tileArray = outArray;
}

void AUnitBase::ResetUnit()
{
	iUnitActionPoints = iMaxActionPoints;
	bCanAttack = true;
}

void AUnitBase::UpdateConditions()
{
	TArray<TObjectPtr<UUnitCondition_Base>> persistingConditions;
	for (TObjectPtr<UUnitCondition_Base> condition : Conditions)
	{
		if (condition->OnConditionPersist())
		{
			persistingConditions.Add(condition);
		}
	}
	Conditions = persistingConditions;
}

void AUnitBase::AddCondition(UUnitCondition_Base* condition, int duration, int potency)
{
	condition->OnConditionApplied(this, duration, potency);
	Conditions.Add(condition);
}

void AUnitBase::RemoveCondition(UUnitCondition_Base* condition)
{
	if (Conditions.Contains(condition))
	{
		condition->OnConditionRemoved();
		Conditions.Remove(condition);
	}
}

bool AUnitBase::IsInSafeZone()
{
	// Check if on a safe tile
	TObjectPtr<UHexTile_Creation> tile = Cast<UHexTile_Creation>(CurrentTile);
	if(!tile) return false;

	TSubclassOf<UHexTile_Creation> teamTile = myTeam->CreationTileType;
	if(!teamTile) return false;

	if(tile.IsA(teamTile)) return true;

	return false;
}

#pragma region LifeAndDeath

void AUnitBase::ChangeCurrentHealth(int change)
{
	iCurrentHealth += change;
	
	if (iCurrentHealth >= iMaxHealth)
		iCurrentHealth = iMaxHealth;

	if (iCurrentHealth <= 0)
		bIsAlive = false;
}

void AUnitBase::ReceiveDamage(int amount)
{
	ChangeCurrentHealth(-amount);
	if (iCurrentHealth <= 0)
	{
		SetUnitDead();
	}
}

void AUnitBase::ReceiveHealing(int amount)
{
	ChangeCurrentHealth(amount);
	if (iCurrentHealth > iMaxHealth)
	{
		iCurrentHealth = iMaxHealth;
	}
}

//if can use action return true and pay the AP cost
bool AUnitBase::CanAffordAction(int actionCost)
{
	if (actionCost <= iUnitActionPoints)
		return true;
	return false;
}

void AUnitBase::SetUnitDead()
{
	bIsAlive = false;
	OnUnitDeath.Broadcast();
	GetCurrentTile()->SetOccupyingUnit(nullptr);
	CurrentTile = nullptr;
	this->SetActorHiddenInGame(true);
	this->SetActorLocation(FVector(0,0,-1000));
}

void AUnitBase::SetUnitAlive()
{
	bIsAlive = true;
	this->SetActorHiddenInGame(false);
	// if(myTeam->GetUnits().Num() < 4)
	// {
	// 	myTeam->AddUnit(this);
	// }
	
}

#pragma endregion 
