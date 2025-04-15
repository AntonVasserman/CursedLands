// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLAnimInstance.h"
#include "Characters/CLCharacterMovementComponent.h"
#include "CLPlayerCharacterAnimInstance.generated.h"

class ACLPlayerCharacter;
enum class ECLMovementWalkingMode : uint8;
enum class ECLPlayerCharacterMovementMode : uint8;

UENUM(BlueprintType)
enum class ECLCardinalDirection : uint8
{
	Forward		UMETA(DisplayName = "Forward"),
	Backward	UMETA(DisplayName = "Backward"),
	Right		UMETA(DisplayName = "Right"),
	Left		UMETA(DisplayName = "Left"),
};

UENUM(BlueprintType)
enum class ECLGait : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Walking		UMETA(DisplayName = "Walking"),
	Jogging		UMETA(DisplayName = "Jogging"),
	Sprinting	UMETA(DisplayName = "Sprinting"),
};

USTRUCT(BlueprintType)
struct FCLCardinalDirectionAnimation
{
	GENERATED_BODY()

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

private:
	const TMap<ECLMovementWalkingMode, ECLGait> MovementModeGaitMap =
	{
		{ ECLMovementWalkingMode::Idle, ECLGait::Idle },
		{ ECLMovementWalkingMode::Walking, ECLGait::Walking },
		{ ECLMovementWalkingMode::Jogging, ECLGait::Jogging },
		{ ECLMovementWalkingMode::Sprinting, ECLGait::Sprinting },
	};

	UPROPERTY()
	TObjectPtr<ACLPlayerCharacter> PlayerCharacter;

	ECLCardinalDirection EvaluateCardinalDirection(const float InCardinalDirectionAngle, const float InBackwardMin, const float InBackwardMax,
		const float InForwardMin, const float InForwardMax, ECLCardinalDirection InCardinalDirection, float InDeadzone);
	void UpdateLocomotionData(const ACLPlayerCharacter* InPlayerCharacter);
	
	//~ UCLAnimInstance Begin
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//~ UCLAnimInstance End
};
