// Copyright Anton Vasserman, All Rights Reserved.


#include "Controllers/CL_PlayerController.h"

#include "CL_LogChannels.h"
#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/CL_PlayerCharacter.h"
#include "Components/StateTreeComponent.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/CL_UserWidget.h"
#include "UI/HUD/CL_HUD.h"

#if WITH_EDITOR
void ACL_PlayerController::RequestSlomoStarted()
{
	bSlomoRequested = true;
	UE_LOG(LogCL, Display, TEXT("Slomo Update Requested"));
	// TODO: Generate a proper key for the DebugMessage or remove the DebugMessage entirely???
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Slomo Update Requested")));
}

void ACL_PlayerController::RequestSlomoTriggered(const FInputActionValue& InValue)
{
	if (!bSlomoRequested)
	{
		return;
	}
	
	const float SlomoInput = InValue.Get<float>() > 0 ? 0.25f : -0.25f;
	const float CurrentSlomoValue = GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation();
	const float NewSlomoValue = FMath::Clamp(CurrentSlomoValue + SlomoInput, 0.25f, 1.0f);
	
	GetWorld()->GetWorldSettings()->SetTimeDilation(NewSlomoValue);
	UE_LOG(LogCL, Display, TEXT("Slomo Updated from: '%f', to: '%f'"), CurrentSlomoValue, NewSlomoValue);
	// TODO: Generate a proper key for the DebugMessage or remove the DebugMessage entirely???
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Slomo Updated from: '%f', to: '%f'"), CurrentSlomoValue, NewSlomoValue));
	
	bSlomoRequested = false;
}
#endif

void ACL_PlayerController::RequestMoveAction_Gamepad(const FInputActionValue& InValue)
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
		RequestToggleWalkAction();
	}
	else if (PossessedPlayerCharacter->GetCLCharacterMovement()->GetGait() == ECL_Gait::Walking && MovementSpeed >= WalkToJogInputThreshold)
	{
		RequestToggleWalkAction();
	}
	
	// Call regular move action
	AddMovementVector(MovementVector);
}

void ACL_PlayerController::RequestMoveAction_Keyboard(const FInputActionValue& InValue)
{
	if (!PossessedPlayerCharacter->CanMove())
	{
		return;
	}

	const FVector2D MovementVector = InValue.Get<FVector2D>();
	AddMovementVector(MovementVector);
}

void ACL_PlayerController::AddMovementVector(const FVector2D& InMovementVector2D)
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

UInputMappingContext* ACL_PlayerController::GetCurrentInputMappingContext() const
{
	switch (CurrentInputType)
	{
		case ECommonInputType::MouseAndKeyboard:
			return DefaultMouseAndKeyboardMappingContext;
		case ECommonInputType::Gamepad:
			return DefaultGamepadMappingContext;
		default:
			checkNoEntry();
			return nullptr;
	}
}

void ACL_PlayerController::OnInputMethodChanged(ECommonInputType NewInputType)
{
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	// This call should be fine even if a mapping context wasn't added, due to TMap not throwing on 'Remove' for a missing KVP
	EnhancedInputSubsystem->RemoveMappingContext(GetCurrentInputMappingContext());
	
	switch (NewInputType)
	{
	case ECommonInputType::MouseAndKeyboard:
		if (CurrentInputType == ECommonInputType::Gamepad)
		{
			if (PossessedPlayerCharacter->GetCLCharacterMovement()->GetGait() == ECL_Gait::Walking)
			{
				// Return to Jogging
				RequestToggleWalkAction();
			}
		}
		EnhancedInputSubsystem->AddMappingContext(DefaultMouseAndKeyboardMappingContext, 0);
		break;
	case ECommonInputType::Gamepad:
		EnhancedInputSubsystem->AddMappingContext(DefaultGamepadMappingContext, 0);
		break;
	default:
		checkNoEntry();
	}

	CurrentInputType = NewInputType;
}

void ACL_PlayerController::RequestLookAction(const FInputActionValue& InValue)
{
	if (!PossessedPlayerCharacter->CanLook())
	{
		return;
	}
	
	const FVector2D LookAxisVector = InValue.Get<FVector2D>();
	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void ACL_PlayerController::RequestToggleWalkAction()
{
	// In Crouching Stance the default is Walking, so we don't support toggling out of it
	if (PossessedPlayerCharacter->IsCrouching())
	{
		return;
	}
	
	if (PossessedPlayerCharacter->IsWalking())
	{
		PossessedPlayerCharacter->UnWalk();
	}
	else if (PossessedPlayerCharacter->CanWalk())
	{
		PossessedPlayerCharacter->Walk();
	}
}

void ACL_PlayerController::RequestToggleSprintAction()
{
	if (PossessedPlayerCharacter->IsSprinting())
	{
		PossessedPlayerCharacter->UnSprint();
	}
	else if (PossessedPlayerCharacter->CanSprint())
	{
		PossessedPlayerCharacter->Sprint();
	}
}

void ACL_PlayerController::RequestToggleCrouchAction()
{
	if (PossessedPlayerCharacter->IsCrouching())
	{
		PossessedPlayerCharacter->UnCrouch();
	}
	else if (PossessedPlayerCharacter->CanCrouch())
	{
		PossessedPlayerCharacter->Crouch();
	}
}

void ACL_PlayerController::RequestJumpAction()
{
	if (PossessedPlayerCharacter->CanJump())
	{
		PossessedPlayerCharacter->Jump();
	}
}

void ACL_PlayerController::RequestTraverseAction()
{
	if (PossessedPlayerCharacter->CanTraverse())
	{
		PossessedPlayerCharacter->Traverse();
	}
}

void ACL_PlayerController::RequestSlideAction()
{
	if (PossessedPlayerCharacter->CanSlide())
	{
		PossessedPlayerCharacter->Slide();
	}
}

void ACL_PlayerController::RequestPauseMenuAction()
{
	if (PauseMenuWidget == nullptr)
	{
		checkf(PauseMenuWidgetClass, TEXT("PauseMenuWidgetClass uninitialized in object: %s"), *GetFullName());
		PauseMenuWidget = CreateWidget<UCL_UserWidget>(this, PauseMenuWidgetClass);
	}
	
	TogglePauseMenu();
}

void ACL_PlayerController::TogglePauseMenu()
{
	if (!bInPausedMenu)
	{
		PauseMenuWidget->AddToViewport();
		SetShowMouseCursor(true);
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(PauseMenuWidget->TakeWidget());
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(FInputModeGameAndUI());
		UGameplayStatics::SetGamePaused(this, true);
		bInPausedMenu = true;
	}
	else
	{
		UGameplayStatics::SetGamePaused(this, false);
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
		PauseMenuWidget->RemoveFromParent();
		bInPausedMenu = false;
	}
}

//~ APlayerController Begin

void ACL_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Enhanced Input Setup
	checkf(DefaultMouseAndKeyboardMappingContext, TEXT("DefaultMouseAndKeyboardMappingContext uninitialized in object: %s"), *GetFullName());
	checkf(DefaultGamepadMappingContext, TEXT("DefaultGamepadMappingContext uninitialized in object: %s"), *GetFullName());

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	checkf(EnhancedInputSubsystem, TEXT("Enhanced Input isn't set in Project Settings. Failure in object: %s"), *GetFullName());
	
	// Common Input Setup
	UCommonInputSubsystem* CommonInputSubsystem = ULocalPlayer::GetSubsystem<UCommonInputSubsystem>(GetLocalPlayer());
	checkf(CommonInputSubsystem, TEXT("Common Input isn't set in Project Settings. Failure in object: %s"), *GetFullName());
	
	CurrentInputType = CommonInputSubsystem->GetCurrentInputType();
	EnhancedInputSubsystem->AddMappingContext(GetCurrentInputMappingContext(), 0);
	CommonInputSubsystem->OnInputMethodChangedNative.AddUObject(this, &ACL_PlayerController::OnInputMethodChanged);
}

void ACL_PlayerController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);

	PossessedPlayerCharacter = CastChecked<ACL_PlayerCharacter>(PawnToPossess);
	PossessedPlayerCharacter->GetGameplayCamera()->ActivateCameraForPlayerController(this);
	
	ACL_HUD* CLHUD = CastChecked<ACL_HUD>(GetHUD());
	CLHUD->InitOverlay();
}

void ACL_PlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	
	PossessedPlayerCharacter->GetGameplayCamera()->DeactivateCamera();
	PossessedPlayerCharacter = nullptr;
}

void ACL_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

#if WITH_EDITOR
	EnhancedInputComponent->BindAction(SlomoAction, ETriggerEvent::Started, this, &ACL_PlayerController::RequestSlomoStarted);
	EnhancedInputComponent->BindAction(SlomoAction, ETriggerEvent::Triggered, this, &ACL_PlayerController::RequestSlomoTriggered);
#endif
	
	checkf(LookAction, TEXT("LookAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACL_PlayerController::RequestLookAction);
	checkf(MoveAction_Gamepad, TEXT("MoveAction_Gamepad uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(MoveAction_Gamepad, ETriggerEvent::Triggered, this, &ACL_PlayerController::RequestMoveAction_Gamepad);
	checkf(MoveAction_Keyboard, TEXT("MoveAction_Keyboard uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(MoveAction_Keyboard, ETriggerEvent::Triggered, this, &ACL_PlayerController::RequestMoveAction_Keyboard);
	checkf(ToggleCrouchAction, TEXT("ToggleCrouchAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(ToggleCrouchAction, ETriggerEvent::Started, this, &ACL_PlayerController::RequestToggleCrouchAction);
	checkf(ToggleWalkAction, TEXT("ToggleWalkAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(ToggleWalkAction, ETriggerEvent::Started, this, &ACL_PlayerController::RequestToggleWalkAction);
	checkf(ToggleSprintAction, TEXT("ToggleSprintAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(ToggleSprintAction, ETriggerEvent::Started, this, &ACL_PlayerController::RequestToggleSprintAction);
	checkf(JumpAction, TEXT("JumpAction uninitialized in object: %s"), *GetFullName())
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACL_PlayerController::RequestJumpAction);
	checkf(TraverseAction, TEXT("TraverseAction uninitialized in object: %s"), *GetFullName())
	EnhancedInputComponent->BindAction(TraverseAction, ETriggerEvent::Started, this, &ACL_PlayerController::RequestTraverseAction);
	checkf(SlideAction, TEXT("SlideAction uninitialized in object: %s"), *GetFullName())
	EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Started, this, &ACL_PlayerController::RequestSlideAction);
	checkf(PauseMenuAction, TEXT("PauseMenuAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(PauseMenuAction, ETriggerEvent::Started, this, &ACL_PlayerController::RequestPauseMenuAction);
}

//~ APlayerController End
