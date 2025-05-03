// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLAnimInstance.h"
#include "CLPlayerCharacterAnimInstance.generated.h"

enum class ECLStance : uint8;
struct FFloatSpringState;
class ACLPlayerCharacter;
enum class ECLCardinalDirection : uint8;
enum class ECLGait : uint8;
enum class ECLMovementWalkingMode : uint8;
enum class ECLPlayerCharacterMovementMode : uint8;

UENUM(BlueprintType)
enum class ECLRootYawOffsetMode : uint8
{
	BlendOut	UMETA(DisplayName = "Blend Out"),
	Hold		UMETA(DisplayName = "Hold"),
	Accumulate	UMETA(DisplayName = "Accumulate"),
};

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
	float CardinalDirectionAngleWithOffset;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECLCardinalDirection LastCardinalDirection;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECLCardinalDirection CardinalDirection;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECLStance Stance;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	bool bStanceChanged;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	bool bStanceTransition;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECLGait Gait;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	bool bGaitChanged;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion SM Data", Meta = (AllowPrivateAccess = "true"))
	FVector PivotAcceleration2D;
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion SM Data", Meta = (AllowPrivateAccess = "true"))
	float LastPivotTime = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion SM Data", Meta = (AllowPrivateAccess = "true"))
	ECLCardinalDirection PivotCardinalDirection;
	
	UPROPERTY(BlueprintReadOnly, Category = "Rotation Data", Meta = (AllowPrivateAccess = "true"))
	float LeanAngle;
	FRotator PlayerCharacterRotation;
	float LastYawDelta;
	
	UPROPERTY(BlueprintReadOnly, Category = "Root Yaw Offset Data", Meta = (AllowPrivateAccess = "true"))
	float RootYawOffset;
	UPROPERTY(BlueprintReadWrite, Category = "Root Yaw Offset SM Data", Meta = (AllowPrivateAccess = "true"))
	ECLRootYawOffsetMode RootYawOffsetMode = ECLRootYawOffsetMode::BlendOut;
	UPROPERTY(BlueprintReadWrite, Category = "Root Yaw Offset SM Data", Meta = (AllowPrivateAccess = "true"))
	float TurnYawCurveValue;

	bool bDoingTraverseAction;
	bool bFinishedTraversalAction;

private:
	const FName RootRotationYawCurveName = TEXT("root_rotation_Z");
	const FName IsTurningCurveName = TEXT("IsTurning");
	
	UPROPERTY()
	TObjectPtr<ACLPlayerCharacter> PlayerCharacter;
	uint8 bFirstThreadSafeUpdate : 1 = true;

	virtual void UpdateFallData() override;
	void UpdateTraversalData();
	void UpdateAccelerationData(const ACLPlayerCharacter* InPlayerCharacter);
	void UpdateLocomotionData(const ACLPlayerCharacter* InPlayerCharacter);
	void UpdateRootYawOffset(const float DeltaSeconds, const ACLPlayerCharacter* InPlayerCharacter);
	void SetRootYawOffset(const float InRootYawOffset);
	UFUNCTION(BlueprintCallable, Category = "Root Yaw Offset", Meta = (AllowPrivateAccess = "true", BlueprintThreadSafe))
	void ProcessTurnYawCurve();

	// Note that Rotation Data depends on the Locomotion data (on both Gait and Cardinal Direction) 
	void UpdateRotationData(const float DeltaSeconds, const ACLPlayerCharacter* InPlayerCharacter);
	
	//~ UCLAnimInstance Begin
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//~ UCLAnimInstance End
};
