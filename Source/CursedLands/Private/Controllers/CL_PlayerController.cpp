// Copyright Anton Vasserman, All Rights Reserved.


#include "Controllers/CL_PlayerController.h"

#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "EnhancedInputSubsystems.h"
#include "UI/HUD/CL_HUD.h"

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
		EnhancedInputSubsystem->AddMappingContext(DefaultMouseAndKeyboardMappingContext, 0);
		break;
	case ECommonInputType::Gamepad:
		EnhancedInputSubsystem->AddMappingContext(DefaultGamepadMappingContext, 0);
		break;
	default:
		checkNoEntry();
	}

	const ECommonInputType PreviousInputType = CurrentInputType;
	CurrentInputType = NewInputType;

	InputMethodChanged(PreviousInputType);
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

	ACL_HUD* CLHUD = CastChecked<ACL_HUD>(GetHUD());
	CLHUD->InitPrimaryGameLayout();
}

//~ APlayerController End
