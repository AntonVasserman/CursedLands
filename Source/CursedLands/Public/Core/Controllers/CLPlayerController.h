// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CLPlayerController.generated.h"

class ACLCharacter;
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

	//~ APlayerController Begin
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* PawnToPossess) override;
	virtual void OnUnPossess() override;
	virtual void SetupInputComponent() override;
	//~ APlayerController End
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	TObjectPtr<ACLCharacter> PossessedCharacter;
};
