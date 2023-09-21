
#include "GameplaySystems/Team.h"
#include "GameplaySystems/TWS_GameManager.h"
#include "Units/UnitBase.h"

void ATeam::BeginPlay()
{
	Super::BeginPlay();
	GameManager = GetWorld()->GetSubsystem<UTWS_GameManager>();
	GameManager->OnTurnChanged.AddDynamic( this, &ATeam::OnTurnChanged );
}


TObjectPtr<AUnitBase> ATeam::SpawnUnit(TSubclassOf<AUnitBase> UnitType)
{
	if(UnitType == nullptr) return nullptr;
	
	const TObjectPtr<AUnitBase> Unit = GetWorld()->SpawnActor<AUnitBase>(UnitType);
	Unit->SetTeam(this);
	Unit->OnUnitDeath.AddDynamic(this, &ATeam::RemoveDeadUnits);
	Units.Add(Unit);
	OnUnitsChanged.Broadcast();
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

void ATeam::AllUnitsDead()
{
	GameManager->CheckForWin();
}

void ATeam::OnTurnChanged()
{
	for (TObjectPtr<AUnitBase> Unit : Units)
	{
		Unit->ResetUnit();
	}
}