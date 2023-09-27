
#pragma once

#include "CoreMinimal.h"
#include "Units/Unit_Devil.h"
#include "Unit_Devil_Exploder.generated.h"

class UExploder_ExplosionAction;

UCLASS()
class GP4_TEAM02_API AUnit_Devil_Exploder : public AUnit_Devil
{
	GENERATED_BODY()

public:
	AUnit_Devil_Exploder();

protected:
	TObjectPtr<UExploder_ExplosionAction> explosionAction;

	virtual void SetUnitDead() override;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Unit Stats")
	int iExplosionDamage = 2;

	UFUNCTION(BlueprintCallable)
	UExploder_ExplosionAction* GetExplodeAction() const {return explosionAction;}
	
};
