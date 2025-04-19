// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLAnimInstance.h"
#include "CLPlayerCharacterAnimInstance.generated.h"

class ACLPlayerCharacter;
enum class ECLCardinalDirection : uint8;
enum class ECLGait : uint8;
enum class ECLMovementWalkingMode : uint8;
enum class ECLPlayerCharacterMovementMode : uint8;

USTRUCT(BlueprintType)
struct FCLDirectionalAnimation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimSequenceBase> ForwardAnimSequence;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimSequenceBase> BackwardAnimSequence;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimSequenceBase> RightAnimSequence;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimSequenceBase> LeftAnimSequence;

	FCLDirectionalAnimation()
		: ForwardAnimSequence(nullptr), BackwardAnimSequence(nullptr), RightAnimSequence(nullptr), LeftAnimSequence(nullptr)
	{
	}
};

USTRUCT(BlueprintType)
struct FCLFallAnimation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimSequenceBase> LightFallAnimSequence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimSequenceBase> HeavyFallAnimSequence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimSequenceBase> DeadlyFallAnimSequence;

	FCLFallAnimation()
		: LightFallAnimSequence(nullptr), HeavyFallAnimSequence(nullptr), DeadlyFallAnimSequence(nullptr)
	{
	}
};

UCLASS()
class CURSEDLANDS_API UCLPlayerCharacterAnimInstance : public UCLAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helper Functions|Locomotion", Meta = (BlueprintThreadSafe))
	bool IsMovingPerpendicularToPivot() const;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Fall Data", Meta = (AllowPrivateAccess = "true"))
	float FallHeight = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Fall Data|Constants", Meta = (AllowPrivateAccess = "true"))
	float FallHeightForMinFallDamage;
	UPROPERTY(BlueprintReadOnly, Category = "Fall Data|Constants", Meta = (AllowPrivateAccess = "true"))
	float FallHeightForMaxFallDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Acceleration Data", Meta = (AllowPrivateAccess = "true"))
	FVector Acceleration;
	UPROPERTY(BlueprintReadOnly, Category = "Acceleration Data", Meta = (AllowPrivateAccess = "true"))
	FVector Acceleration2D;
	UPROPERTY(BlueprintReadOnly, Category = "Acceleration Data", Meta = (AllowPrivateAccess = "true"))
	bool bAccelerating;
	UPROPERTY(BlueprintReadOnly, Category = "Acceleration Data", Meta = (AllowPrivateAccess = "true"))
	float AccelerationAngle;
	
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECLPlayerCharacterMovementMode MovementMode;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	float CardinalDirectionAngle;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECLCardinalDirection LastCardinalDirection;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECLCardinalDirection CardinalDirection;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	bool bGaitChanged;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECLGait Gait;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion SM Data", Meta = (AllowPrivateAccess = "true"))
	FVector PivotAcceleration2D;
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion SM Data", Meta = (AllowPrivateAccess = "true"))
	ECLCardinalDirection PivotCardinalDirection;
	
	UPROPERTY(BlueprintReadOnly, Category = "Rotation Data", Meta = (AllowPrivateAccess = "true"))
	float LeanAngle;
	FRotator PlayerCharacterRotation;
	float LastYawDelta;

private:
	UPROPERTY()
	TObjectPtr<ACLPlayerCharacter> PlayerCharacter;
	uint8 bFirstThreadSafeUpdate : 1 = true;

	virtual void UpdateFallData() override;
	void UpdateAccelerationData(const ACLPlayerCharacter* InPlayerCharacter);
	void UpdateLocomotionData(const ACLPlayerCharacter* InPlayerCharacter);

	// Note that Rotation Data depends on the Locomotion data (on both Gait and Cardinal Direction) 
	void UpdateRotationData(const float DeltaSeconds, const ACLPlayerCharacter* InPlayerCharacter);
	
	//~ UCLAnimInstance Begin
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//~ UCLAnimInstance End
};
