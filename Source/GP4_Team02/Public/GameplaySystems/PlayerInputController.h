// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerInputController.generated.h"

//Forward declaration
class ATeam;
class APlayerCursor;

UCLASS()
class GP4_TEAM02_API APlayerInputController : public APlayerController
{
	GENERATED_BODY()

public:
	// APlayerInputController();
	
	void SetupPlayerInputComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveCursorAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SelectAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* DeselectAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* InfoAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ToggleMenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<APlayerCursor>PlayerCursor;

	UFUNCTION(BlueprintPure)
	ATeam* GetTeam() const { return myTeam; }
	
	void AssignTeam(TObjectPtr<ATeam> team) { myTeam = team; }

protected:
	virtual void BeginPlay();

	void MoveCursor(const FInputActionValue& Value);
	
	void Select();
	
	void Deselect();

	void Info();
	
	void ToggleMenu();

private:
	TObjectPtr<APlayerCursor> Cursor;

	TObjectPtr<ATeam> myTeam;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite , meta = (AllowPrivateAccess = "true"))
	FVector CursorSpawnLocation = FVector(0,0,0);
};
