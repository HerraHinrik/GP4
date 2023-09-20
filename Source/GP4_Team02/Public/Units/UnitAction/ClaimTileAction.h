
#pragma once

#include "CoreMinimal.h"
#include "Units/UnitAction.h"
#include "ClaimTileAction.generated.h"

class UTile_ClaimableHexTile;

UCLASS()
class GP4_TEAM02_API UClaimTileAction : public UUnitAction
{
	GENERATED_BODY()
	
TObjectPtr<UTile_ClaimableHexTile> ClaimableTile = nullptr;


public:
	virtual void StartAction(UTileBase* tile, AUnitBase* unit) override;
	virtual void ExecuteAction() override;
	virtual void EndAction() override;
};
