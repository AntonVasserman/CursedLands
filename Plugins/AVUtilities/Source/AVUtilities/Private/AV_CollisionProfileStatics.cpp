// Copyright Anton Vasserman, All Rights Reserved.

#include "AV_CollisionProfileStatics.h"
#include "Engine/CollisionProfile.h"

AVUTILITIES_API const FName UAV_CollisionProfileStatics::NoCollision_ProfileName = UCollisionProfile::NoCollision_ProfileName;
AVUTILITIES_API const FName UAV_CollisionProfileStatics::BlockAll_ProfileName = UCollisionProfile::BlockAll_ProfileName;
AVUTILITIES_API const FName UAV_CollisionProfileStatics::OverlapAll_ProfileName = FName(TEXT("OverlapAll"));
AVUTILITIES_API const FName UAV_CollisionProfileStatics::BlockAllDynamic_ProfileName = UCollisionProfile::BlockAllDynamic_ProfileName;
AVUTILITIES_API const FName UAV_CollisionProfileStatics::OverlapAllDynamic_ProfileName = FName(TEXT("OverlapAllDynamic"));
AVUTILITIES_API const FName UAV_CollisionProfileStatics::IgnoreOnlyPawn_ProfileName = FName(TEXT("IgnoreOnlyPawn"));
AVUTILITIES_API const FName UAV_CollisionProfileStatics::OverlapOnlyPawn_ProfileName = FName(TEXT("OverlapOnlyPawn"));
AVUTILITIES_API const FName UAV_CollisionProfileStatics::Pawn_ProfileName = UCollisionProfile::Pawn_ProfileName;
AVUTILITIES_API const FName UAV_CollisionProfileStatics::Spectator_ProfileName = FName(TEXT("Spectator"));
AVUTILITIES_API const FName UAV_CollisionProfileStatics::CharacterMesh_ProfileName = FName(TEXT("CharacterMesh"));
AVUTILITIES_API const FName UAV_CollisionProfileStatics::PhysicsActor_ProfileName = UCollisionProfile::PhysicsActor_ProfileName;
AVUTILITIES_API const FName UAV_CollisionProfileStatics::Destructible_ProfileName = FName(TEXT("Destructible"));
AVUTILITIES_API const FName UAV_CollisionProfileStatics::InvisibleWall_ProfileName = FName(TEXT("InvisibleWall"));
AVUTILITIES_API const FName UAV_CollisionProfileStatics::InvisibleWallDynamic_ProfileName = FName(TEXT("InvisibleWallDynamic"));
AVUTILITIES_API const FName UAV_CollisionProfileStatics::Trigger_ProfileName = FName(TEXT("Trigger"));
AVUTILITIES_API const FName UAV_CollisionProfileStatics::Ragdoll_ProfileName = FName(TEXT("Ragdoll"));
AVUTILITIES_API const FName UAV_CollisionProfileStatics::Vehicle_ProfileName = UCollisionProfile::Vehicle_ProfileName;
AVUTILITIES_API const FName UAV_CollisionProfileStatics::UI_ProfileName = FName(TEXT("UI"));
AVUTILITIES_API const FName UAV_CollisionProfileStatics::DefaultProjectile_ProfileName = UCollisionProfile::DefaultProjectile_ProfileName;
