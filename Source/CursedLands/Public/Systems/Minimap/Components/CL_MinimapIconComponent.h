// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CL_MinimapIconComponent.generated.h"

UCLASS(Meta=(BlueprintSpawnableComponent))
class CURSEDLANDS_API UCL_MinimapIconComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Config|Minimap")
	TSoftClassPtr<UTexture2D> IconTextureClass;

public:
	UFUNCTION(BlueprintCallable, Category = "Minimap")
	FORCEINLINE TSubclassOf<UTexture2D> GetIconTexture() const { return IconTextureClass.LoadSynchronous(); }
};
