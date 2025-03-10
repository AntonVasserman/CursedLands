// Copyright Anton Vasserman, All Rights Reserved.


#include "Core/Controllers/CLPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Core/Characters/CLCharacter.h"

void ACLPlayerController::RequestMoveAction(const FInputActionValue& InValue)
{
	const FVector2D MovementVector = InValue.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (PossessedCharacter)
	{
		PossessedCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
		PossessedCharacter->AddMovementInput(RightDirection, MovementVector.X);
	}
	else
	{
		// TODO: Should we do something?
	}
}

void ACLPlayerController::RequestLookAction(const FInputActionValue& InValue)
{
	const FVector2D LookAxisVector = InValue.Get<FVector2D>();
	if (PossessedCharacter)
	{
		PossessedCharacter->AddControllerYawInput(LookAxisVector.X);
		PossessedCharacter->AddControllerPitchInput(LookAxisVector.Y);	
	}
	else
	{
		// TODO: Should we do something?
	}
}

void ACLPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(DefaultMappingContext);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	else
	{
		// TODO: Should we check anything here?
	}
}

void ACLPlayerController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);

	PossessedCharacter = Cast<ACLCharacter>(PawnToPossess);
}

void ACLPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

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
