// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "AVCollisionProfileStatics.generated.h"

UCLASS(MinimalAPI)
class UAVCollisionProfileStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static AVUTILITIES_API const FName NoCollision_ProfileName;
	static AVUTILITIES_API const FName BlockAll_ProfileName;
	static AVUTILITIES_API const FName OverlapAll_ProfileName;
	static AVUTILITIES_API const FName BlockAllDynamic_ProfileName;
	static AVUTILITIES_API const FName OverlapAllDynamic_ProfileName;
	static AVUTILITIES_API const FName IgnoreOnlyPawn_ProfileName;
	static AVUTILITIES_API const FName OverlapOnlyPawn_ProfileName;
	static AVUTILITIES_API const FName Pawn_ProfileName;
	static AVUTILITIES_API const FName Spectator_ProfileName;
	static AVUTILITIES_API const FName CharacterMesh_ProfileName;
	static AVUTILITIES_API const FName PhysicsActor_ProfileName;
	static AVUTILITIES_API const FName Destructible_ProfileName;
	static AVUTILITIES_API const FName InvisibleWall_ProfileName;
	static AVUTILITIES_API const FName InvisibleWallDynamic_ProfileName;
	static AVUTILITIES_API const FName Trigger_ProfileName;
	static AVUTILITIES_API const FName Ragdoll_ProfileName;
	static AVUTILITIES_API const FName Vehicle_ProfileName;
	static AVUTILITIES_API const FName UI_ProfileName;
	static AVUTILITIES_API const FName DefaultProjectile_ProfileName;
};
