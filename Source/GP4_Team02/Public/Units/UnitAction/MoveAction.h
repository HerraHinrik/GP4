
#pragma once

#include "CoreMinimal.h"
#include "Units/UnitAction.h"
#include "MoveAction.generated.h"

class ULink;

UCLASS()
class GP4_TEAM02_API UMoveAction : public UUnitAction
{
	GENERATED_BODY()
	
public:
	virtual void StartAction(UTileBase* tile, AUnitBase* unit) override;

protected:
	virtual void ExecuteAction() override;
	virtual void EndAction() override;

private:
	TObjectPtr<ULink> MoveLink = nullptr;
	TArray<TObjectPtr<ULink>> Path;

	int32 iTilesMoved = 0;
	
};
