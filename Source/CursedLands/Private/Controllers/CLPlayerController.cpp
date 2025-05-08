// Copyright Anton Vasserman, All Rights Reserved.


#include "Controllers/CLPlayerController.h"

#include "CLLogChannels.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/CLPlayerCharacter.h"
#include "Components/StateTreeComponent.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CLUserWidget.h"
#include "UI/HUD/CLHUD.h"

#if WITH_EDITOR
ACLPlayerController::ACLPlayerController()
{
	// TODO (CL-130): Create a StateTree Schema for CLPlayerController and CLPlayerCharacter
	PlayerStateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("PlayerStateTree"));
}

void ACLPlayerController::RequestSlomoStarted()
{
	bSlomoRequested = true;
	UE_LOG(LogCL, Display, TEXT("Slomo Update Requested"));
	// TODO: Generate a proper key for the DebugMessage or remove the DebugMessage entirely???
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Slomo Update Requested")));
}

void ACLPlayerController::RequestSlomoTriggered(const FInputActionValue& InValue)
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

	// In case Player Character is in strafing movement mode, sprinting and changes direction from forward direction then stop sprinting
	if (
		PossessedPlayerCharacter->GetMovementMode() == ECLPlayerCharacterMovementMode::Strafing &&
		PossessedPlayerCharacter->IsSprinting() &&
		PossessedPlayerCharacter->GetCardinalDirection() != ECLCardinalDirection::Forward
		)
	{
		PossessedPlayerCharacter->UnSprint();
	}
	
	PossessedPlayerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	PossessedPlayerCharacter->AddMovementInput(RightDirection, MovementVector.X);
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

void ACLPlayerController::RequestToggleWalkAction()
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

void ACLPlayerController::RequestToggleSprintAction()
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

void ACLPlayerController::RequestToggleCrouchAction()
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

void ACLPlayerController::RequestJumpAction()
{
	if (PossessedPlayerCharacter->CanJump())
	{
		PossessedPlayerCharacter->Jump();
	}
}

void ACLPlayerController::RequestTraverseAction()
{
	if (PossessedPlayerCharacter->CanTraverse())
	{
		PossessedPlayerCharacter->Traverse();
	}
}

void ACLPlayerController::RequestSlideAction()
{
	if (PossessedPlayerCharacter->CanSlide())
	{
		PossessedPlayerCharacter->Slide();
	}
}

void ACLPlayerController::RequestPauseMenuAction()
{
	if (PauseMenuWidget == nullptr)
	{
		checkf(PauseMenuWidgetClass, TEXT("PauseMenuWidgetClass uninitialized in object: %s"), *GetFullName());
		PauseMenuWidget = CreateWidget<UCLUserWidget>(this, PauseMenuWidgetClass);
	}
	
	TogglePauseMenu();
}

void ACLPlayerController::TogglePauseMenu()
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

void ACLPlayerController::OnPossessedPlayerCharacterFellToRoll()
{
	// No need to check the feedback here for nullptr, as it is allowed to be nullptr in ClientPlayForceFeedback.
	ClientPlayForceFeedback(FallToRollForceFeedbackEffect);
}

void ACLPlayerController::OnPossessedPlayerCharacterFellToDeath()
{
	// No need to check the feedback here for nullptr, as it is allowed to be nullptr in ClientPlayForceFeedback.
	ClientPlayForceFeedback(FallToDeathForceFeedbackEffect);
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
	PossessedPlayerCharacter->OnFellToRoll.AddDynamic(this, &ACLPlayerController::OnPossessedPlayerCharacterFellToRoll);
	PossessedPlayerCharacter->OnFellToDeath.AddDynamic(this, &ACLPlayerController::OnPossessedPlayerCharacterFellToDeath);
	
	ACLHUD* CLHUD = CastChecked<ACLHUD>(GetHUD());
	CLHUD->InitOverlay();
}

void ACLPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	PossessedPlayerCharacter->OnFellToRoll.RemoveDynamic(this, &ACLPlayerController::OnPossessedPlayerCharacterFellToRoll);
	PossessedPlayerCharacter->OnFellToDeath.RemoveDynamic(this, &ACLPlayerController::OnPossessedPlayerCharacterFellToDeath);
	PossessedPlayerCharacter->GetGameplayCamera()->DeactivateCamera();
	PossessedPlayerCharacter = nullptr;
}

void ACLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

#if WITH_EDITOR
	EnhancedInputComponent->BindAction(SlomoAction, ETriggerEvent::Started, this, &ACLPlayerController::RequestSlomoStarted);
	EnhancedInputComponent->BindAction(SlomoAction, ETriggerEvent::Triggered, this, &ACLPlayerController::RequestSlomoTriggered);
#endif
	
	checkf(LookAction, TEXT("LookAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACLPlayerController::RequestLookAction);
	checkf(MoveAction, TEXT("MoveAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACLPlayerController::RequestMoveAction);
	checkf(ToggleCrouchAction, TEXT("ToggleCrouchAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(ToggleCrouchAction, ETriggerEvent::Started, this, &ACLPlayerController::RequestToggleCrouchAction);
	checkf(ToggleWalkAction, TEXT("ToggleWalkAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(ToggleWalkAction, ETriggerEvent::Started, this, &ACLPlayerController::RequestToggleWalkAction);
	checkf(ToggleSprintAction, TEXT("ToggleSprintAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(ToggleSprintAction, ETriggerEvent::Started, this, &ACLPlayerController::RequestToggleSprintAction);
	checkf(JumpAction, TEXT("JumpAction uninitialized in object: %s"), *GetFullName())
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACLPlayerController::RequestJumpAction);
	checkf(TraverseAction, TEXT("TraverseAction uninitialized in object: %s"), *GetFullName())
	EnhancedInputComponent->BindAction(TraverseAction, ETriggerEvent::Started, this, &ACLPlayerController::RequestTraverseAction);
	checkf(SlideAction, TEXT("SlideAction uninitialized in object: %s"), *GetFullName())
	EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Started, this, &ACLPlayerController::RequestSlideAction);
	checkf(PauseMenuAction, TEXT("PauseMenuAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(PauseMenuAction, ETriggerEvent::Started, this, &ACLPlayerController::RequestPauseMenuAction);
}

//~ APlayerController End
