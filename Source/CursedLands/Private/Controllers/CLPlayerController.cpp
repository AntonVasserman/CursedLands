// Copyright Anton Vasserman, All Rights Reserved.


#include "Controllers/CLPlayerController.h"

#include "CLGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/CLPlayerCharacter.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CLUserWidget.h"
#include "UI/HUD/CLHUD.h"

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
	
	// On Strafing movement mode we need to adjust the Pawn rotation on movement.
	// This is because we are using the Gameplay Camera instead of a regular Camera
	if (
		PossessedPlayerCharacter->GetMovementMode() == ECLPlayerCharacterMovementMode::Strafing ||
		PossessedPlayerCharacter->HasMatchingGameplayTag(CLGameplayTags::Locomotion_Rolling)
		)
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
		PossessedPlayerCharacter->UnSprint();
	}
	else if (PossessedPlayerCharacter->CanSprint())
	{
		PossessedPlayerCharacter->Sprint();
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

	checkf(LookAction, TEXT("LookAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACLPlayerController::RequestLookAction);
	checkf(MoveAction, TEXT("MoveAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACLPlayerController::RequestMoveAction);
	checkf(ToggleSprintAction, TEXT("ToggleSprintAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(ToggleSprintAction, ETriggerEvent::Started, this, &ACLPlayerController::RequestToggleSprintAction);
	checkf(JumpAction, TEXT("JumpAction uninitialized in object: %s"), *GetFullName())
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACLPlayerController::RequestJumpAction);
	checkf(TraverseAction, TEXT("TraverseAction uninitialized in object: %s"), *GetFullName())
	EnhancedInputComponent->BindAction(TraverseAction, ETriggerEvent::Started, this, &ACLPlayerController::RequestTraverseAction);
	checkf(PauseMenuAction, TEXT("PauseMenuAction uninitialized in object: %s"), *GetFullName());
	EnhancedInputComponent->BindAction(PauseMenuAction, ETriggerEvent::Started, this, &ACLPlayerController::RequestPauseMenuAction);
}

//~ APlayerController End
