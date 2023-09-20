
#pragma once

#include "CoreMinimal.h"
#include "UnitAction_Offensive.h"
#include "AttackAction.generated.h"

class ULink;

UCLASS()
class GP4_TEAM02_API UAttackAction : public UUnitAction_Offensive
{
	GENERATED_BODY()
	


public:
	virtual void StartAction(UTileBase* tile, AUnitBase* unit) override;

protected:
	virtual void ExecuteAction() override;
	virtual void EndAction() override;

private:
	TObjectPtr<ULink> moveLink = nullptr;
};
