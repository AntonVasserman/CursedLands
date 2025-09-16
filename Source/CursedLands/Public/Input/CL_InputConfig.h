// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CL_InputConfig.generated.h"

class UInputAction;

UCLASS(BlueprintType, Const)
class CURSEDLANDS_API UCL_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", Meta = (ForceInlineRow, Categories = "InputTag"))
	TMap<FGameplayTag, TObjectPtr<UInputAction>> NativeInputActionsByTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", Meta = (ForceInlineRow, Categories = "InputTag.Ability"))
	TMap<FGameplayTag, TObjectPtr<UInputAction>> AbilityInputActionsByTag;
	
	UFUNCTION(BlueprintCallable, Category = "Pawn")
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	
	UFUNCTION(BlueprintCallable, Category = "Pawn")
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
};
