
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TeamPawn.generated.h"

class ATeam;
class UTWS_GameManager;
UCLASS()
class GP4_TEAM02_API ATeamPawn : public APawn
{
	GENERATED_BODY()

public:
	ATeamPawn();

protected:
	bool bIsMyTurn = false;

	TObjectPtr<ATeam> myTeam;
	TObjectPtr<UTWS_GameManager> GameManager;

	
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void virtual CheckIfMyTurn();

	UFUNCTION(BlueprintCallable)
	void SetMyTurn(bool isMYTurn) { bIsMyTurn = isMYTurn; }

};
