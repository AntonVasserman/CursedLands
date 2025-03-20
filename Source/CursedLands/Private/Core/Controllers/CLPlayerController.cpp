// Copyright Anton Vasserman, All Rights Reserved.


#include "Core/Controllers/CLPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Core/Characters/CLPlayerCharacter.h"
#include "Core/HUDs/CLHUD.h"
#include "GameFramework/GameplayCameraComponent.h"

void ACLPlayerController::RequestMoveAction(const FInputActionValue& InValue)
{
	if (!PossessedPlayerCharacter)
	{
		return;
	}

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	const FVector2D MovementVector = InValue.Get<FVector2D>();
	PossessedPlayerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	PossessedPlayerCharacter->AddMovementInput(RightDirection, MovementVector.X);
}

void ACLPlayerController::RequestLookAction(const FInputActionValue& InValue)
{
	if (!PossessedPlayerCharacter)
	{
		return;
	}
	
	const FVector2D LookAxisVector = InValue.Get<FVector2D>();
	PossessedPlayerCharacter->AddControllerYawInput(LookAxisVector.X);
	PossessedPlayerCharacter->AddControllerPitchInput(LookAxisVector.Y);
}


void ACLPlayerController::RequestToggleSprint()
{
	if (PossessedPlayerCharacter->CanSprint())
	{
		PossessedPlayerCharacter->ToggleSprint();
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
	CLHUD->InitOverlay(PossessedPlayerCharacter->GetAbilitySystem(), PossessedPlayerCharacter->GetAttributeSet());
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
	EnhancedInputComponent->BindAction(ToggleSprintAction, ETriggerEvent::Started, this, &ACLPlayerController::RequestToggleSprint);
}

//~ APlayerController End
