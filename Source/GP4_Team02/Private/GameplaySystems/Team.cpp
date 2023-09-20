
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
	return nullptr;
}

AUnitBase* ATeam::SpawnUnit(int32 UnitTier)
{
	return nullptr;
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