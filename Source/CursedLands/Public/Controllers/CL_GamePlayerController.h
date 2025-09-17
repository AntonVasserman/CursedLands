// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_PlayerController.h"
#include "CL_GamePlayerController.generated.h"

UCLASS()
class CURSEDLANDS_API ACL_GamePlayerController : public ACL_PlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCL_InputConfig> InputConfig;
	
	UPROPERTY()
	TObjectPtr<ACL_PlayerCharacter> PossessedPlayerCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Movement", Meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float WalkToJogInputThreshold = 0.4f;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input|Actions|MouseAndKeyboard|Debug", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SlomoAction;
	
	bool bSlomoRequested = false;
#endif
	
	void RequestMoveAction_Gamepad(const FInputActionValue& InValue);
	void RequestMoveAction_KeyboardAndMouse(const FInputActionValue& InValue);
	void RequestLookAction(const FInputActionValue& InValue);
	
	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);
	
	void AddMovementVector(const FVector2D& InMovementVector2D);
#if WITH_EDITOR
	void RequestSlomoStarted();
	void RequestSlomoTriggered(const FInputActionValue& InValue);
#endif
	
	//~ ACL_PlayerController Begin
public:
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
protected:
	virtual void OnPossess(APawn* PawnToPossess) override;
	virtual void OnUnPossess() override;
	virtual void SetupInputComponent() override;
private:
	virtual void InputMethodChanged(ECommonInputType NewInputType) override;
	//~ ACL_PlayerController End
};
