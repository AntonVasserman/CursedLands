// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CL_PlayerCharacterCameraMode.h"
#include "Characters/Components/CL_ExtendedCharacterMovementComponent.h"
#include "Directors/CameraDirectorStateTreeSchema.h"
#include "UObject/Object.h"

#include "CL_GetPlayerCharacterCameraInfoGCSTTask.generated.h"

enum class ECL_PlayerCharacterCameraMode : uint8;
enum class ECL_Stance : uint8;
class ACL_PlayerCharacter;

USTRUCT()
struct CURSEDLANDS_API FCL_GetPlayerCharacterCameraInfoGCSTTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY()
	ACL_PlayerCharacter* PlayerCharacterRef = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Outputs")
	ECL_Stance Stance = ECL_Stance::Standing;

	UPROPERTY(EditAnywhere, Category = "Outputs")
	ECL_PlayerCharacterCameraMode CameraMode = ECL_PlayerCharacterCameraMode::Default;

	UPROPERTY(EditAnywhere, Category = "Outputs")
	bool bSprinting = false;
};

USTRUCT(Meta = (DisplayName = "Get Player Character Camera Info", Category = "Cameras"))
struct CURSEDLANDS_API FCL_GetPlayerCharacterCameraInfoGCSTTask : public FGameplayCamerasStateTreeTask
{
	GENERATED_BODY()

	using FInstanceDataType = FCL_GetPlayerCharacterCameraInfoGCSTTaskInstanceData;

	FCL_GetPlayerCharacterCameraInfoGCSTTask() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	
	//~ FGameplayCamerasStateTreeTask Begin
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	//~ FGameplayCamerasStateTreeTask End
};
