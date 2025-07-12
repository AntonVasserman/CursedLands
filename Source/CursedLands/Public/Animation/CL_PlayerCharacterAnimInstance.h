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
};
