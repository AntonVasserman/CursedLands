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
struct FCLCardinalDirectionAnimation
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

	FCLCardinalDirectionAnimation()
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

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data", Meta = (AllowPrivateAccess = "true"))
	ECLPlayerCharacterMovementMode MovementMode;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data", Meta = (AllowPrivateAccess = "true"))
	float FallHeight = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data | Constants", Meta = (AllowPrivateAccess = "true"))
	float FallHeightForMinFallDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data | Constants", Meta = (AllowPrivateAccess = "true"))
	float FallHeightForMaxFallDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	float CardinalDirectionAngle;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECLCardinalDirection CardinalDirection;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECLGait Gait;
	
	UPROPERTY(BlueprintReadOnly, Category = "Rotation Data", Meta = (AllowPrivateAccess = "true"))
	float LeanAngle;
	FRotator PlayerCharacterRotation;
	float LastYawDelta;

private:
	UPROPERTY()
	TObjectPtr<ACLPlayerCharacter> PlayerCharacter;
	uint8 bFirstUpdate : 1 = true;
	
	void UpdateLocomotionData(const ACLPlayerCharacter* InPlayerCharacter);

	// Note that Rotation Data depends on the Locomotion data (on both Gait and Cardinal Direction) 
	void UpdateRotationData(const float DeltaSeconds, const ACLPlayerCharacter* InPlayerCharacter);
	
	//~ UCLAnimInstance Begin
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//~ UCLAnimInstance End
};
