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

	UPROPERTY(BlueprintReadOnly, Category = "Location Data", Meta = (AllowPrivateAccess = "true"))
	FVector LastCharacterLocation;
	UPROPERTY(BlueprintReadOnly, Category = "Location Data", Meta = (AllowPrivateAccess = "true"))
	FVector CharacterLocation;
	UPROPERTY(BlueprintReadOnly, Category = "Location Data", Meta = (AllowPrivateAccess = "true"))
	float CharacterLocationDeltaSizeXY;
	UPROPERTY(BlueprintReadOnly, Category = "Location Data", Meta = (AllowPrivateAccess = "true"))
	float CharacterLocationDeltaSizeXYSpeed;

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
	uint8 bFirstThreadSafeUpdate : 1 = true;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	void UpdateLocationData(const float DeltaTime);
	void UpdateVelocityData();
	
	//~ UAnimInstance Begin
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	//~ UAnimInstance End
	
};
