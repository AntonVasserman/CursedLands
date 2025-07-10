// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CL_AnimInstance.generated.h"

class ACL_Character;
class UCharacterMovementComponent;

UCLASS()
class CURSEDLANDS_API UCL_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
	FORCEINLINE UCharacterMovementComponent* GetCharacterMovementComponent() { return MovementComponent;}
	
protected:
	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	bool bAlive = true;

	UPROPERTY(BlueprintReadWrite, Category = "Location Data", Meta = (AllowPrivateAccess = "true"))
	FVector LastCharacterLocation;
	UPROPERTY(BlueprintReadWrite, Category = "Location Data", Meta = (AllowPrivateAccess = "true"))
	FVector CharacterLocation;
	UPROPERTY(BlueprintReadWrite, Category = "Location Data", Meta = (AllowPrivateAccess = "true"))
	float CharacterLocationDeltaSizeXY;
	UPROPERTY(BlueprintReadWrite, Category = "Location Data", Meta = (AllowPrivateAccess = "true"))
	float CharacterLocationDeltaSizeXYSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Velocity Data", Meta = (AllowPrivateAccess = "true"))
	FVector Velocity;

	UPROPERTY(BlueprintReadWrite, Category = "Velocity Data", Meta = (AllowPrivateAccess = "true"))
	FVector Velocity2D;

	UPROPERTY(BlueprintReadWrite, Category = "Velocity Data", Meta = (AllowPrivateAccess = "true"))
	float Velocity2DSize;

	UPROPERTY(BlueprintReadWrite, Category = "Fall Data", Meta = (AllowPrivateAccess = "true"))
	bool bFalling = false;

	virtual void UpdateFallData();

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ACL_Character> Character;
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
