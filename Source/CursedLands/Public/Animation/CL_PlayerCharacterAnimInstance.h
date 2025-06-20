// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_AnimInstance.h"
#include "Systems/Falling/CL_FallType.h"
#include "CL_PlayerCharacterAnimInstance.generated.h"

enum class ECL_Stance : uint8;
struct FFloatSpringState;
class ACL_PlayerCharacter;
enum class ECL_CardinalDirection : uint8;
enum class ECL_Gait : uint8;
enum class ECL_PlayerCharacterMovementMode : uint8;

UENUM(BlueprintType)
enum class ECL_RootYawOffsetMode : uint8
{
	BlendOut	UMETA(DisplayName = "Blend Out"),
	Hold		UMETA(DisplayName = "Hold"),
	Accumulate	UMETA(DisplayName = "Accumulate"),
};

USTRUCT(BlueprintType)
struct FCL_DirectionalAnimation
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

	FCL_DirectionalAnimation()
		: ForwardAnimSequence(nullptr), BackwardAnimSequence(nullptr), RightAnimSequence(nullptr), LeftAnimSequence(nullptr)
	{
	}
};

USTRUCT(BlueprintType)
struct FCL_FallAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimSequenceBase> LightFallAnimSequence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimSequenceBase> HeavyFallAnimSequence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimSequenceBase> DeadlyFallAnimSequence;

	FCL_FallAnimation()
		: LightFallAnimSequence(nullptr), HeavyFallAnimSequence(nullptr), DeadlyFallAnimSequence(nullptr)
	{
	}
};

UCLASS()
class CURSEDLANDS_API UCL_PlayerCharacterAnimInstance : public UCL_AnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helper Functions|Locomotion", Meta = (BlueprintThreadSafe))
	bool IsMovingPerpendicularToPivot() const;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Fall Data", Meta = (AllowPrivateAccess = "true"))
	float FallHeight = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Fall Data", Meta = (AllowPrivateAccess = "true"))
	bool bRolling;
	UPROPERTY(BlueprintReadOnly, Category = "Fall Data", Meta = (AllowPrivateAccess = "true"))
	bool bStandingUp;
	UPROPERTY(BlueprintReadOnly, Category = "Fall Data|Constants", Meta = (AllowPrivateAccess = "true"))
	ECL_FallType FallType;

	UPROPERTY(BlueprintReadOnly, Category = "Acceleration Data", Meta = (AllowPrivateAccess = "true"))
	FVector Acceleration;
	UPROPERTY(BlueprintReadOnly, Category = "Acceleration Data", Meta = (AllowPrivateAccess = "true"))
	FVector Acceleration2D;
	UPROPERTY(BlueprintReadOnly, Category = "Acceleration Data", Meta = (AllowPrivateAccess = "true"))
	bool bAccelerating;
	UPROPERTY(BlueprintReadOnly, Category = "Acceleration Data", Meta = (AllowPrivateAccess = "true"))
	float AccelerationAngle;
	
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECL_PlayerCharacterMovementMode MovementMode;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	float CardinalDirectionAngle;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	float CardinalDirectionAngleWithOffset;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECL_CardinalDirection LastCardinalDirection;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECL_CardinalDirection CardinalDirection;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECL_Stance Stance;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	bool bStanceChanged;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	bool bStanceTransition;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECL_Gait Gait;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	bool bGaitChanged;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion SM Data", Meta = (AllowPrivateAccess = "true"))
	FVector PivotAcceleration2D;
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion SM Data", Meta = (AllowPrivateAccess = "true"))
	float LastPivotTime = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion SM Data", Meta = (AllowPrivateAccess = "true"))
	ECL_CardinalDirection PivotCardinalDirection;
	
	UPROPERTY(BlueprintReadOnly, Category = "Rotation Data", Meta = (AllowPrivateAccess = "true"))
	float LeanAngle;
	FRotator PlayerCharacterRotation;
	float LastYawDelta;
	
	UPROPERTY(BlueprintReadOnly, Category = "Root Yaw Offset Data", Meta = (AllowPrivateAccess = "true"))
	float RootYawOffset;
	UPROPERTY(BlueprintReadWrite, Category = "Root Yaw Offset SM Data", Meta = (AllowPrivateAccess = "true"))
	ECL_RootYawOffsetMode RootYawOffsetMode = ECL_RootYawOffsetMode::BlendOut;
	UPROPERTY(BlueprintReadWrite, Category = "Root Yaw Offset SM Data", Meta = (AllowPrivateAccess = "true"))
	float TurnYawCurveValue;

	bool bDoingTraverseAction;
	bool bFinishedTraversalAction;

private:
	const FName RootRotationYawCurveName = TEXT("root_rotation_Z");
	const FName IsTurningCurveName = TEXT("IsTurning");
	
	UPROPERTY()
	TObjectPtr<ACL_PlayerCharacter> PlayerCharacter;
	uint8 bFirstThreadSafeUpdate : 1 = true;

	virtual void UpdateFallData() override;
	void UpdateTraversalData();
	void UpdateAccelerationData(const ACL_PlayerCharacter* InPlayerCharacter);
	void UpdateLocomotionData(const ACL_PlayerCharacter* InPlayerCharacter);
	void UpdateRootYawOffset(const float DeltaSeconds, const ACL_PlayerCharacter* InPlayerCharacter);
	void SetRootYawOffset(const float InRootYawOffset);
	UFUNCTION(BlueprintCallable, Category = "Root Yaw Offset", Meta = (AllowPrivateAccess = "true", BlueprintThreadSafe))
	void ProcessTurnYawCurve();

	// Note that Rotation Data depends on the Locomotion data (on both Gait and Cardinal Direction) 
	void UpdateRotationData(const float DeltaSeconds, const ACL_PlayerCharacter* InPlayerCharacter);
	
	//~ UCLAnimInstance Begin
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//~ UCLAnimInstance End
};
