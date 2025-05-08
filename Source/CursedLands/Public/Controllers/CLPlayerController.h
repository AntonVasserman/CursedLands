// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CLPlayerController.generated.h"

class ACLPlayerCharacter;
class UCLUserWidget;
class UInputAction;
class UInputMappingContext;
class UStateTreeComponent;
struct FInputActionValue;

UCLASS()
class CURSEDLANDS_API ACLPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StateTree", Meta = (AllowPrivateAccess = "true"))
	UStateTreeComponent* PlayerStateTreeComponent;

public:
	ACLPlayerController();
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SlomoAction;
#endif
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	// This action is used only for Keyboard. With a controller we don't toggle walking, it's blended together with jogging
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
	TSubclassOf<UCLUserWidget> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement|Falling|ForceFeedback", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UForceFeedbackEffect> FallToRollForceFeedbackEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement|Falling|ForceFeedback", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UForceFeedbackEffect> FallToDeathForceFeedbackEffect;
	
#if WITH_EDITOR
	bool bSlomoRequested = false;
#endif
	
	TObjectPtr<ACLPlayerCharacter> PossessedPlayerCharacter;
	TObjectPtr<UCLUserWidget> PauseMenuWidget = nullptr;
	bool bInPausedMenu = false;

#if WITH_EDITOR
	void RequestSlomoStarted();
	void RequestSlomoTriggered(const FInputActionValue& InValue);
#endif
	
	void RequestMoveAction(const FInputActionValue& InValue);
	void RequestLookAction(const FInputActionValue& InValue);
	void RequestToggleWalkAction();
	void RequestToggleSprintAction();
	void RequestToggleCrouchAction();
	void RequestJumpAction();
	void RequestTraverseAction();
	void RequestSlideAction();
	void RequestPauseMenuAction();

	UFUNCTION(BlueprintCallable, Category = "Menus")
	void TogglePauseMenu();

	UFUNCTION(Category = "Character Movement|Falling")
	void OnPossessedPlayerCharacterFellToRoll();
	UFUNCTION(Category = "Character Movement|Falling")
	void OnPossessedPlayerCharacterFellToDeath();
	
	//~ APlayerController Begin
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* PawnToPossess) override;
	virtual void OnUnPossess() override;
	virtual void SetupInputComponent() override;
	//~ APlayerController End

};
