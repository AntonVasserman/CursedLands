// Copyright Anton Vasserman, All Rights Reserved.


#include "Core/Controllers/CLPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Core/Characters/CLCharacter.h"
#include "GameFramework/GameplayCameraComponent.h"

void ACLPlayerController::RequestMoveAction(const FInputActionValue& InValue)
{
	if (!PossessedCharacter)
	{
		return;
	}

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	const FVector2D MovementVector = InValue.Get<FVector2D>();
	PossessedCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	PossessedCharacter->AddMovementInput(RightDirection, MovementVector.X);
}

void ACLPlayerController::RequestLookAction(const FInputActionValue& InValue)
{
	if (!PossessedCharacter)
	{
		return;
	}
	
	const FVector2D LookAxisVector = InValue.Get<FVector2D>();
	PossessedCharacter->AddControllerYawInput(LookAxisVector.X);
	PossessedCharacter->AddControllerPitchInput(LookAxisVector.Y);
}

void ACLPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(DefaultMappingContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	
	Subsystem->AddMappingContext(DefaultMappingContext, 0);
}

void ACLPlayerController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);

	PossessedCharacter = Cast<ACLCharacter>(PawnToPossess);
	PossessedCharacter->GetGameplayCamera()->ActivateCameraForPlayerController(this);
}

void ACLPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	PossessedCharacter->GetGameplayCamera()->DeactivateCamera();
	PossessedCharacter = nullptr;
}

void ACLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	check(LookAction);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACLPlayerController::RequestLookAction);
	check(MoveAction);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACLPlayerController::RequestMoveAction);
}
