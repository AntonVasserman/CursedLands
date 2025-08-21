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
	TObjectPtr<UTexture2D> IconTexture;

public:
	UFUNCTION(BlueprintCallable, Category = "Minimap")
	FORCEINLINE UTexture2D* GetIconTexture() const { return IconTexture; }

	UFUNCTION(BlueprintCallable, Category = "Minimap")
	FORCEINLINE void SetIconTexture(UTexture2D* InIconTexture) { IconTexture = InIconTexture; }
};
