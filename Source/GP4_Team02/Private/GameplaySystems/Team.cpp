
#include "GameplaySystems/Team.h"
#include "GameplaySystems/TWS_GameManager.h"
#include "Units/UnitBase.h"

void ATeam::BeginPlay()
{
	Super::BeginPlay();
	GameManager = GetWorld()->GetSubsystem<UTWS_GameManager>();
	GameManager->OnTurnChanged.AddDynamic( this, &ATeam::OnTurnChanged );
	OnUnitsChanged.AddDynamic(this, &ATeam::CheckEndTurn);
}


TObjectPtr<AUnitBase> ATeam::SpawnUnit(TSubclassOf<AUnitBase> UnitType)
{
	if(UnitType == nullptr) return nullptr;
	if (Units.Num() >= MaxUnitsSpawned) return nullptr;
	if(iCreationPoints <= 0) return nullptr;
	
	const TObjectPtr<AUnitBase> Unit = GetWorld()->SpawnActor<AUnitBase>(UnitType);
	Unit->SetTeam(this);
	Unit->OnUnitDeath.AddDynamic(this, &ATeam::RemoveDeadUnits);
	Units.Add(Unit);
	OnUnitsChanged.Broadcast();
	iCreationPoints--;
	return Unit;
}

AUnitBase* ATeam::SpawnUnit(int32 UnitTier)
{
	if(UnitTypes.Num() == 0) return nullptr;
	// Clamp the tier to the number of unit types
	UnitTier = FMath::Clamp(UnitTier, 0, UnitTypes.Num() - 1);
	return SpawnUnit(UnitTypes[UnitTier]);	
}

TArray<TObjectPtr<AUnitBase>> ATeam::SpawnStartUnits()
{
	return TArray<TObjectPtr<AUnitBase>>();
}

void ATeam::RemoveDeadUnits()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Death");
	TArray<TObjectPtr<AUnitBase>> LivingUnits;
	for (TObjectPtr<AUnitBase> unit : Units)
	{
		if (!unit)
			continue;
		
		if(unit->IsUnitAlive())
		{
			LivingUnits.Add(unit);
		}
		else
		{
			UnitPool.Add(unit);
		}
	}
	
	if (LivingUnits.IsEmpty())
	{
		AllUnitsDead();
		Units.Empty();
	}
	else
	{
		Units = LivingUnits;
	}
	OnUnitsChanged.Broadcast();
}

void ATeam::UpdateUnitConditions()
{
	if (bWasMyTurn)
	{
		for (TObjectPtr<AUnitBase> Unit : Units)
		{
			Unit->UpdateConditions();
		}
	}
	bWasMyTurn = false;
}

void ATeam::AllUnitsDead()
{
	GameManager->CheckForWin();
}

void ATeam::OnTurnChanged()
{
	// if our turn has started, reset our units and creation points
	if (GameManager->GetCurrentTeam() == this)
	{
		for (TObjectPtr<AUnitBase> Unit : Units)
		{
			Unit->ResetUnit();
		}
		iCreationPoints = iMaxCreationPoints;
	}
}

void ATeam::CheckEndTurn()
{
	// Check for creationpoints first
	if (iCreationPoints > 0)
	{
		return;
	}
	// Check if any units have action points left
	bool bEndTurn = true;
	for (const TObjectPtr<AUnitBase> Unit : Units)
	{
		if(Unit->GetRemainingActionPoints() > 0)
		{
			bEndTurn = false;
			break;
		}
	}
	if(bEndTurn)
	{
		GetWorld()->GetSubsystem<UTWS_GameManager>()->EndTurn();
	}
}