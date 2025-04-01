// Copyright Anton Vasserman, All Rights Reserved.


#include "Controllers/CLPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/CLPlayerCharacter.h"
#include "UI/HUD/CLHUD.h"
#include "GameFramework/GameplayCameraComponent.h"

void ACLPlayerController::RequestMoveAction(const FInputActionValue& InValue)
{
	if (!PossessedPlayerCharacter->CanMove())
	{
		return;
	}

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	const FVector2D MovementVector = InValue.Get<FVector2D>();

	// In case Player Character is sprinting and changes direction from forward direction then stop sprinting
	if (MovementVector.Y < 0.5 && PossessedPlayerCharacter->IsSprinting())
	{
		PossessedPlayerCharacter->UnToggleSprint();
	}
	
	PossessedPlayerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	PossessedPlayerCharacter->AddMovementInput(RightDirection, MovementVector.X);
	
	// On Strafing movement mode we need to adjust the Pawn rotation on movement.
	// This is because we are using the Gameplay Camera instead of regular Camera
	if (PossessedPlayerCharacter->GetMovementMode() == ECLPlayerCharacterMovementMode::Strafing)
	{
		FRotator NewRotation = PossessedPlayerCharacter->GetActorRotation();
		NewRotation.Yaw = GetControlRotation().Yaw;
		PossessedPlayerCharacter->SetActorRotation(NewRotation);
	}
}

void ACLPlayerController::RequestLookAction(const FInputActionValue& InValue)
{
	if (!PossessedPlayerCharacter->CanLook())
	{
		return;
	}
	
	const FVector2D LookAxisVector = InValue.Get<FVector2D>();
	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void ACLPlayerController::RequestToggleSprintAction()
{
	if (PossessedPlayerCharacter->IsSprinting())
	{
		PossessedPlayerCharacter->UnToggleSprint();
	}
	else if (PossessedPlayerCharacter->CanSprint())
	{
		PossessedPlayerCharacter->ToggleSprint();
	}
}

void ACLPlayerController::RequestJumpAction()
{
	// TODO: Consider overriding CanJump so we can check if Character is alive...
	if (PossessedPlayerCharacter->CanJump())
	{
		PossessedPlayerCharacter->Jump();
	}
}

//~ APlayerController Begin

void ACLPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	checkf(DefaultMappingContext, TEXT("DefaultMappingContext uninitialized in object: %s"), *GetFullName());

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	checkf(Subsystem, TEXT("Enhanced Input isn't set in Project Settings. Failure in object: %s"), *GetFullName());
	
	Subsystem->AddMappingContext(DefaultMappingContext, 0);
}

void ACLPlayerController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);

	PossessedPlayerCharacter = CastChecked<ACLPlayerCharacter>(PawnToPossess);
	PossessedPlayerCharacter->GetGameplayCamera()->ActivateCameraForPlayerController(this);
	ACLHUD* CLHUD = CastChecked<ACLHUD>(GetHUD());
	CLHUD->InitOverlay();
}

void ACLPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	PossessedPlayerCharacter->GetGameplayCamera()->DeactivateCamera();
	PossessedPlayerCharacter = nullptr;
}

void ACLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	checkf(LookAction, TEXT("LookAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACLPlayerController::RequestLookAction);
	checkf(MoveAction, TEXT("MoveAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACLPlayerController::RequestMoveAction);
	checkf(ToggleSprintAction, TEXT("ToggleSprintAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(ToggleSprintAction, ETriggerEvent::Started, this, &ACLPlayerController::RequestToggleSprintAction);
	checkf(JumpAction, TEXT("JumpAction uninitialized in object: %s"), *GetFullName())
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACLPlayerController::RequestJumpAction);
}

//~ APlayerController End
