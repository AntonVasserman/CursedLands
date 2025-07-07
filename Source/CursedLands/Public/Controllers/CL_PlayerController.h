// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CL_PlayerController.generated.h"

class ACL_PlayerCharacter;
class UCL_UserWidget;
class UInputAction;
class UInputMappingContext;
class UStateTreeComponent;
struct FInputActionValue;

UCLASS()
class CURSEDLANDS_API ACL_PlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StateTree", Meta = (AllowPrivateAccess = "true"))
	UStateTreeComponent* PlayerStateTreeComponent;

public:
	ACL_PlayerController();
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SlomoAction;
#endif

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction_Gamepad;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction_Keyboard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	// This action is used only for Keyboard. With a controller we don't toggle walking, it's should be blended with jogging
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ToggleWalkAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ToggleSprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ToggleCrouchAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> TraverseAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SlideAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PauseMenuAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Menus", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCL_UserWidget> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Movement", Meta = (AllowPrivateAccess = "true"))
	float WalkToJogInputThreshold = 0.4f;
	
#if WITH_EDITOR
	bool bSlomoRequested = false;
#endif

	UPROPERTY()
	TObjectPtr<ACL_PlayerCharacter> PossessedPlayerCharacter;
	UPROPERTY()
	TObjectPtr<UCL_UserWidget> PauseMenuWidget = nullptr;
	bool bInPausedMenu = false;
	// TODO (CL-177): Transition to use the EnhancedInput's 'OnInputDeviceChanged' event to understand if Gamepad/Keyboard was changed...
	bool bLastUsedKeyboard = true;

#if WITH_EDITOR
	void RequestSlomoStarted();
	void RequestSlomoTriggered(const FInputActionValue& InValue);
#endif

	void RequestMoveAction_Gamepad(const FInputActionValue& InValue);
	void RequestMoveAction_Keyboard(const FInputActionValue& InValue);
	void RequestLookAction(const FInputActionValue& InValue);
	void RequestToggleWalkAction();
	void RequestToggleSprintAction();
	void RequestToggleCrouchAction();
	void RequestJumpAction();
	void RequestTraverseAction();
	void RequestSlideAction();
	void RequestPauseMenuAction();

	void AddMovementVector(const FVector2D& InMovementVector2D);
	UFUNCTION(BlueprintCallable, Category = "Menus")
	void TogglePauseMenu();

	//~ APlayerController Begin
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* PawnToPossess) override;
	virtual void OnUnPossess() override;
	virtual void SetupInputComponent() override;
	//~ APlayerController End

};
