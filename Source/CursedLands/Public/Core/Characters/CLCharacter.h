// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CLCharacter.generated.h"

class UCameraComponent;
class UGameplayCameraComponent;
class USpringArmComponent;

UCLASS()
class CURSEDLANDS_API ACLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACLCharacter();

	FORCEINLINE UGameplayCameraComponent* GetGameplayCamera() const { return GameplayCamera; }
	
	//~ ACharacter Begin
	virtual void Tick(float DeltaTime) override;
	//~ ACharacter End

protected:
	//~ ACharacter Begin
	virtual void BeginPlay() override;
	//~ ACharacter End

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGameplayCameraComponent> GameplayCamera;
};
