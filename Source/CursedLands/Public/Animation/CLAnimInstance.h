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

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
	FORCEINLINE UCharacterMovementComponent* GetCharacterMovementComponent() { return MovementComponent;}
	
protected:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	bool bAlive = true;
	
	UPROPERTY(BlueprintReadOnly, Category = "Velocity Data", Meta = (AllowPrivateAccess = "true"))
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "Velocity Data", Meta = (AllowPrivateAccess = "true"))
	FVector Velocity2D;

	UPROPERTY(BlueprintReadOnly, Category = "Velocity Data", Meta = (AllowPrivateAccess = "true"))
	float Velocity2DSize;

	UPROPERTY(BlueprintReadOnly, Category = "Fall Data", Meta = (AllowPrivateAccess = "true"))
	bool bFalling = false;

	UFUNCTION(BlueprintCallable, Category = "Character Physics")
	void CharacterMeshSimulatePhysics() const;

	virtual void UpdateFallData();

private:
	UPROPERTY()
	TObjectPtr<ACLCharacter> Character;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	void UpdateVelocityData();
	
	//~ UAnimInstance Begin
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//~ UAnimInstance End
	
};
