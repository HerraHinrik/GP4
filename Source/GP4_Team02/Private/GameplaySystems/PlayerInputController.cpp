// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplaySystems/PlayerInputController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplaySystems/PlayerCursor.h"
#include "GameplaySystems/TWS_GameManager.h"


void APlayerInputController::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetSubsystem<UTWS_GameManager>()->AssignTeamToController(this);
	if (myTeam)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, myTeam->GetTeamName());
	}

	APlayerCursor* SpawnedCursor = GetWorld()->SpawnActor<APlayerCursor>(PlayerCursor, CursorSpawnLocation, FRotator::ZeroRotator);
	SpawnedCursor->SetTeam(myTeam);
	this->Possess(SpawnedCursor);
	
	Cursor = Cast<APlayerCursor>(GetPawn());
	if (Cursor)
	{
		GetWorld()->GetSubsystem<UTWS_GameManager>()->AddPawnToArray(Cursor);
	}
	
	if (!SpawnedCursor || !GetPawn())
	{
		UE_LOG(LogTemp, Warning, TEXT("No cursor!"));
		if (!Cursor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cursor not found"));
		}
	}
	

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	SetupPlayerInputComponent();

	// FInputModeUIOnly InputMode;
	// InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	// SetInputMode(InputMode);
}

// APlayerInputController::APlayerInputController()
// {
// }

void APlayerInputController::SetupPlayerInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveCursorAction, ETriggerEvent::Triggered, this, &APlayerInputController::MoveCursor);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Completed, this, &APlayerInputController::Select);
		EnhancedInputComponent->BindAction(DeselectAction, ETriggerEvent::Completed, this, &APlayerInputController::Deselect);
		EnhancedInputComponent->BindAction(ToggleMenuAction, ETriggerEvent::Completed, this, &APlayerInputController::ClaimTile);
		EnhancedInputComponent->BindAction(InfoAction, ETriggerEvent::Completed, this, &APlayerInputController::Info);
		EnhancedInputComponent->BindAction(ToggleMenuAction, ETriggerEvent::Completed, this, &APlayerInputController::ToggleMenu);
		EnhancedInputComponent->BindAction(EndTurnAction, ETriggerEvent::Completed, this, &APlayerInputController::EndTurn);
		EnhancedInputComponent->BindAction(DestroyUnitAction, ETriggerEvent::Completed, this, &APlayerInputController::DestroyUnit);
	}
}

void APlayerInputController::MoveCursor(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	MovementVector.X = FMath::Clamp(MovementVector.X, -1.0, 1.0);
	MovementVector.Y = FMath::Clamp(MovementVector.Y, -1.0, 1.0);
	if (Cursor)
	{
		Cursor->MovePlayerCursor(MovementVector);
	}
}

void APlayerInputController::Select()
{
	if (Cursor)
	{
		Cursor->SelectTarget();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No cursor!"));
	}
}

void APlayerInputController::Deselect()
{
	if (Cursor)
	{
		Cursor->DeselectTarget();
	}
}

void APlayerInputController::ClaimTile()
{
	if (Cursor)
	{
		Cursor->ClaimTile();
	}
}

void APlayerInputController::Info()
{
	
}

void APlayerInputController::ToggleMenu()
{
	//Toggle Menu
}

void APlayerInputController::EndTurn()
{
	if (Cursor)
	{
		Cursor->EndTurn();
	}
}

void APlayerInputController::DestroyUnit()
{
	if (Cursor)
	{
		Cursor->DestroySelectedUnit();
	}
}

