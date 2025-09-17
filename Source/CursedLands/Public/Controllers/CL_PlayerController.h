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
	
protected:
	virtual void InputMethodChanged(ECommonInputType PreviousType) { }

	ECommonInputType CurrentInputType;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMouseAndKeyboardMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultGamepadMappingContext;

	UInputMappingContext* GetCurrentInputMappingContext() const;

	UFUNCTION()
	void OnInputMethodChanged(ECommonInputType NewInputType);
	//~ APlayerController Begin
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* PawnToPossess) override;
	//~ APlayerController End

};
