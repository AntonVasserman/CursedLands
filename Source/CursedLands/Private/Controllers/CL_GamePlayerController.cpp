// Copyright Anton Vasserman, All Rights Reserved.


#include "Controllers/CL_GamePlayerController.h"

#include "CL_LogChannels.h"
#include "InputActionValue.h"
#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "Characters/CL_PlayerCharacter.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "Input/CL_InputComponent.h"
#include "Systems/Traversal/CL_CharacterTraversalComponent.h"

void ACL_GamePlayerController::RequestMoveAction_Gamepad(const FInputActionValue& InValue)
{
	if (!PossessedPlayerCharacter->CanMove())
	{
		return;
	}

	FVector2D MovementVector = InValue.Get<FVector2D>();
	const float MovementSpeed = MovementVector.Size();

	// If walking isn't supported for the character, we try to normalize movement so that movement vector size is always
	// 1.0, resulting in always Jogging.
	if (PossessedPlayerCharacter->GetCLCharacterMovement()->CanEverWalk() == false)
	{
		if (const float Length = MovementVector.Size();
			Length > KINDA_SMALL_NUMBER) // Prevent division by zero
		{
			MovementVector = MovementVector / Length;
		}
	}
	else if (PossessedPlayerCharacter->GetCLCharacterMovement()->GetGait() == ECL_Gait::Jogging && MovementSpeed < WalkToJogInputThreshold)
	{
		PossessedPlayerCharacter->Walk();
	}
	else if (PossessedPlayerCharacter->GetCLCharacterMovement()->GetGait() == ECL_Gait::Walking && MovementSpeed >= WalkToJogInputThreshold)
	{
		PossessedPlayerCharacter->UnWalk();
	}
	
	// Call regular move action
	AddMovementVector(MovementVector);
}

void ACL_GamePlayerController::RequestMoveAction_KeyboardAndMouse(const FInputActionValue& InValue)
{
	if (!PossessedPlayerCharacter->CanMove())
	{
		return;
	}

	const FVector2D MovementVector = InValue.Get<FVector2D>();
	AddMovementVector(MovementVector);
}

void ACL_GamePlayerController::RequestLookAction(const FInputActionValue& InValue)
{
	if (!PossessedPlayerCharacter->CanLook())
	{
		return;
	}
	
	const FVector2D LookAxisVector = InValue.Get<FVector2D>();
	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void ACL_GamePlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
	UCL_AbilitySystemComponent* AbilitySystemComponent = PossessedPlayerCharacter->GetCLAbilitySystemComponent();
	if (AbilitySystemComponent == nullptr)
	{
		CL_LOG_GAMEPLAY_ABILITY_SYSTEM(Warning, TEXT("AbilitySystemComponent found null PlayerController for PossessedCharacter: %s"), *PossessedPlayerCharacter->GetFullName());
		return;
	}
	
	AbilitySystemComponent->AbilityInputPressed(InputTag);
}

void ACL_GamePlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	UCL_AbilitySystemComponent* AbilitySystemComponent = PossessedPlayerCharacter->GetCLAbilitySystemComponent();
	if (AbilitySystemComponent == nullptr)
	{
		CL_LOG_GAMEPLAY_ABILITY_SYSTEM(Warning, TEXT("AbilitySystemComponent found null PlayerController for PossessedCharacter: %s"), *PossessedPlayerCharacter->GetFullName());
		return;
	}
	
	AbilitySystemComponent->AbilityInputReleased(InputTag);
}

void ACL_GamePlayerController::AddMovementVector(const FVector2D& InMovementVector2D)
{
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	// In case Player Character is in strafing movement mode, sprinting and changes direction from forward direction then stop sprinting
	if (
		PossessedPlayerCharacter->GetMovementMode() == ECL_PlayerCharacterMovementMode::Strafing &&
		PossessedPlayerCharacter->IsSprinting() &&
		PossessedPlayerCharacter->GetCardinalDirection() != ECL_CardinalDirection::Forward
		)
	{
		PossessedPlayerCharacter->UnSprint();
	}
	
	PossessedPlayerCharacter->AddMovementInput(ForwardDirection, InMovementVector2D.Y);
	PossessedPlayerCharacter->AddMovementInput(RightDirection, InMovementVector2D.X);
}

#if WITH_EDITOR
void ACL_GamePlayerController::RequestSlomoStarted()
{
	bSlomoRequested = true;
	CL_LOG(Display, "Slomo Update Requested");
}

void ACL_GamePlayerController::RequestSlomoTriggered(const FInputActionValue& InValue)
{
	if (!bSlomoRequested)
	{
		return;
	}
	
	const float SlomoInput = InValue.Get<float>() > 0 ? 0.25f : -0.25f;
	const float CurrentSlomoValue = GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation();
	const float NewSlomoValue = FMath::Clamp(CurrentSlomoValue + SlomoInput, 0.25f, 1.0f);
	
	GetWorld()->GetWorldSettings()->SetTimeDilation(NewSlomoValue);
	CL_LOG(Display, "Slomo Updated from: '%f', to: '%f'", CurrentSlomoValue, NewSlomoValue);
	
	bSlomoRequested = false;
}

void ACL_GamePlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UCL_AbilitySystemComponent* AbilitySystemComponent = PossessedPlayerCharacter->GetCLAbilitySystemComponent();
		AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->ProcessAbilityInput(DeltaTime, bGamePaused);	
	}
	else
	{
		CL_LOG_GAMEPLAY_ABILITY_SYSTEM(Warning, TEXT("AbilitySystemComponent found null PlayerController for PossessedCharacter: %s"), *PossessedPlayerCharacter->GetFullName());
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

//~ ACL_PlayerController Begin

void ACL_GamePlayerController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);
	
	PossessedPlayerCharacter = CastChecked<ACL_PlayerCharacter>(PawnToPossess);
	PossessedPlayerCharacter->GetGameplayCamera()->ActivateCameraForPlayerController(this);
}

void ACL_GamePlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	
	PossessedPlayerCharacter->GetGameplayCamera()->DeactivateCamera();
	PossessedPlayerCharacter = nullptr;
}
#endif

void ACL_GamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UCL_InputComponent* CLInputComponent = CastChecked<UCL_InputComponent>(InputComponent);

	checkf(InputConfig, TEXT("InputConfig uninitialized in: %s"), *GetFullName());
	CLInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased);

	CLInputComponent->BindNativeAction(InputConfig, CLGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::RequestLookAction);
	CLInputComponent->BindNativeAction(InputConfig, CLGameplayTags::InputTag_Move_Gamepad, ETriggerEvent::Triggered, this, &ThisClass::RequestMoveAction_Gamepad);
	CLInputComponent->BindNativeAction(InputConfig, CLGameplayTags::InputTag_Move_KeyboardAndMouse, ETriggerEvent::Triggered, this, &ThisClass::RequestMoveAction_KeyboardAndMouse);
	
#if WITH_EDITOR
	CLInputComponent->BindAction(SlomoAction, ETriggerEvent::Started, this, &ACL_GamePlayerController::RequestSlomoStarted);
	CLInputComponent->BindAction(SlomoAction, ETriggerEvent::Triggered, this, &ACL_GamePlayerController::RequestSlomoTriggered);
#endif
}

void ACL_GamePlayerController::InputMethodChanged(ECommonInputType PreviousInputType)
{
	switch (CurrentInputType)
	{
	case ECommonInputType::MouseAndKeyboard:
		if (PreviousInputType == ECommonInputType::Gamepad)
		{
			if (PossessedPlayerCharacter->GetCLCharacterMovement()->GetGait() == ECL_Gait::Walking)
			{
				// Return to Jogging
				PossessedPlayerCharacter->UnWalk();
			}
		}
		break;
	case ECommonInputType::Gamepad:
		break;
	default:
		checkNoEntry();
	}
}

//~ ACL_PlayerController End
