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
};
