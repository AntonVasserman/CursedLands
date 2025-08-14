// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "GameFramework/PlayerController.h"
#include "CL_PlayerController.generated.h"

enum class ECommonInputType : uint8;
struct FGameplayTag;
struct FInputActionValue;
class ACL_PlayerCharacter;
class UCL_InputConfig;
class UCL_UserWidget;
class UInputAction;
class UInputMappingContext;

UCLASS()
class CURSEDLANDS_API ACL_PlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMouseAndKeyboardMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultGamepadMappingContext;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input|Actions|MouseAndKeyboard|Debug", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SlomoAction;
#endif
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCL_InputConfig> InputConfig;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Movement", Meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float WalkToJogInputThreshold = 0.4f;
	
#if WITH_EDITOR
	bool bSlomoRequested = false;
#endif

	UPROPERTY()
	TObjectPtr<ACL_PlayerCharacter> PossessedPlayerCharacter = nullptr;
	ECommonInputType CurrentInputType;

#if WITH_EDITOR
	void RequestSlomoStarted();
	void RequestSlomoTriggered(const FInputActionValue& InValue);
#endif

	void RequestMoveAction_Gamepad(const FInputActionValue& InValue);
	void RequestMoveAction_KeyboardAndMouse(const FInputActionValue& InValue);
	void RequestLookAction(const FInputActionValue& InValue);
	void RequestToggleWalkAction();
	void RequestToggleCrouchAction();
	void RequestTraverseAction();
	void RequestSlideAction();

	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);
	
	void AddMovementVector(const FVector2D& InMovementVector2D);
	UInputMappingContext* GetCurrentInputMappingContext() const;
	UFUNCTION()
	void OnInputMethodChanged(ECommonInputType NewInputType);
	
	//~ APlayerController Begin
public:
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* PawnToPossess) override;
	virtual void OnUnPossess() override;
	virtual void SetupInputComponent() override;
	//~ APlayerController End

};
