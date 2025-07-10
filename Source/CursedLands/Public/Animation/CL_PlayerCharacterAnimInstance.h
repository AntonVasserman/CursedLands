// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_AnimInstance.h"
#include "CL_PlayerCharacterAnimInstance.generated.h"

enum class ECL_CardinalDirection : uint8;
enum class ECL_FallType : uint8;
enum class ECL_Gait : uint8;
enum class ECL_PlayerCharacterMovementMode : uint8;
enum class ECL_Stance : uint8;
class ACL_PlayerCharacter;

UCLASS()
class CURSEDLANDS_API UCL_PlayerCharacterAnimInstance : public UCL_AnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Fall Data", Meta = (AllowPrivateAccess = "true"))
	float FallHeight = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "Fall Data", Meta = (AllowPrivateAccess = "true"))
	bool bRolling;
	UPROPERTY(BlueprintReadWrite, Category = "Fall Data", Meta = (AllowPrivateAccess = "true"))
	bool bStandingUp;
	UPROPERTY(BlueprintReadWrite, Category = "Fall Data", Meta = (AllowPrivateAccess = "true"))
	ECL_FallType FallType;

	UPROPERTY(BlueprintReadWrite, Category = "Acceleration Data", Meta = (AllowPrivateAccess = "true"))
	FVector Acceleration;
	UPROPERTY(BlueprintReadWrite, Category = "Acceleration Data", Meta = (AllowPrivateAccess = "true"))
	FVector Acceleration2D;
	UPROPERTY(BlueprintReadWrite, Category = "Acceleration Data", Meta = (AllowPrivateAccess = "true"))
	bool bAccelerating;
	UPROPERTY(BlueprintReadWrite, Category = "Acceleration Data", Meta = (AllowPrivateAccess = "true"))
	float AccelerationAngle;
	
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECL_PlayerCharacterMovementMode MovementMode;
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	float CardinalDirectionAngle;
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	float CardinalDirectionAngleWithOffset;
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECL_CardinalDirection LastCardinalDirection;
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECL_CardinalDirection CardinalDirection;
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECL_Stance Stance;
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	bool bStanceChanged;
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	bool bStanceTransition;
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	ECL_Gait Gait;
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion Data", Meta = (AllowPrivateAccess = "true"))
	bool bGaitChanged;

	UPROPERTY(BlueprintReadWrite, Category = "Rotation Data", Meta = (AllowPrivateAccess = "true"))
	float LeanAngle;
	UPROPERTY(BlueprintReadWrite, Category = "Rotation Data", Meta = (AllowPrivateAccess = "true"))
	FRotator PlayerCharacterRotation;
	UPROPERTY(BlueprintReadWrite, Category = "Rotation Data", Meta = (AllowPrivateAccess = "true"))
	float LastYawDelta;

	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	bool bDoingTraverseAction;
	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	bool bFinishedTraversalAction;

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ACL_PlayerCharacter> PlayerCharacter;
	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	uint8 bFirstThreadSafeUpdate : 1 = true;

	virtual void UpdateFallData() override;
	void UpdateTraversalData();
	void UpdateAccelerationData(const ACL_PlayerCharacter* InPlayerCharacter);

	//~ UCLAnimInstance Begin
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//~ UCLAnimInstance End
};
