// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CLPlayerController.generated.h"

class ACLPlayerCharacter;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class CURSEDLANDS_API ACLPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	void RequestMoveAction(const FInputActionValue& InValue);
	void RequestLookAction(const FInputActionValue& InValue);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ToggleSprintAction;

	TObjectPtr<ACLPlayerCharacter> PossessedPlayerCharacter;

	UFUNCTION()
	void RequestToggleSprint();

	//~ APlayerController Begin

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* PawnToPossess) override;
	virtual void OnUnPossess() override;
	virtual void SetupInputComponent() override;
	
	//~ APlayerController End

};
