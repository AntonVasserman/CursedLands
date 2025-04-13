// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CLAnimInstance.generated.h"

class ACLCharacter;
class UCharacterMovementComponent;

UCLASS()
class CURSEDLANDS_API UCLAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data", Meta = (AllowPrivateAccess = "true"))
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data", Meta = (AllowPrivateAccess = "true"))
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data", Meta = (AllowPrivateAccess = "true"))
	float Direction;
	
	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data", Meta = (AllowPrivateAccess = "true"))
	bool bShouldMove;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data", Meta = (AllowPrivateAccess = "true"))
	bool bAlive = true;
	
	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data", Meta = (AllowPrivateAccess = "true"))
	bool bFalling = false;

	UFUNCTION(BlueprintCallable, Category = "Character Physics")
	void CharacterMeshSimulatePhysics() const;

private:
	UPROPERTY()
	TObjectPtr<ACLCharacter> Character;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
	
	//~ UAnimInstance Begin
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//~ UAnimInstance End
	
};
