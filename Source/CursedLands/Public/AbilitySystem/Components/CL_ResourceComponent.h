// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/CL_ResourceAttributeSet.h"
#include "Components/ActorComponent.h"
#include "CL_ResourceComponent.generated.h"

class UCL_ResourceViewModel;
class UMVVMViewModelBase;
class UCL_AbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCL_OnResourceViewModelInstantiated, UCL_ResourceViewModel*, ResourceViewModel);

UENUM()
enum class ECL_ResourceState : uint8
{
	Full		UMETA(DisplayName = "Full"),
	Normal		UMETA(DisplayName = "Normal"),
	Critical	UMETA(DisplayName = "Critical"),
	Depleted	UMETA(DisplayName = "Depleted"),
};

USTRUCT(BlueprintType)
struct FCL_ResourceGameplayTags
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
	FGameplayTag Full;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
	FGameplayTag Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
	FGameplayTag Critical;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
	FGameplayTag Depleted;
};

UCLASS(Abstract)
class CURSEDLANDS_API UCL_ResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCL_ResourceComponent();

	UPROPERTY(BlueprintAssignable)
	FCL_OnResourceViewModelInstantiated OnResourceViewModelInstantiated;

	UFUNCTION(BlueprintCallable, Category = "Resource")
	virtual void InitializeWithAbilitySystem(UCL_AbilitySystemComponent* InAbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category = "Resource")
	void UnInitializeFromAbilitySystem();

	UFUNCTION(BlueprintCallable, Category = "Resource|ViewModel")
	void InitializeViewModel();
	
	UFUNCTION(BlueprintCallable, Category = "Resource")
	FORCEINLINE float GetValue() const { return ResourceAttributeSet->GetValue(); }

	UFUNCTION(BlueprintCallable, Category = "Resource")
	FORCEINLINE float GetMaxValue() const { return ResourceAttributeSet->GetMaxValue(); }

protected:
	UPROPERTY()
	TObjectPtr<UCL_AbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY()
	TObjectPtr<const UCL_ResourceAttributeSet> ResourceAttributeSet = nullptr;
	TSubclassOf<UCL_ResourceAttributeSet> ResourceAttributeSetClass = nullptr;
	FCL_ResourceGameplayTags ResourceGameplayTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource|ViewModel")
	FName ResourceViewModelContextName;

	UPROPERTY(BlueprintReadOnly, Category = "Resource|ViewModel")
	TObjectPtr<UCL_ResourceViewModel> ResourceViewModel = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Resource", Meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float CriticalSectionThresholdInPercentage = 0.2f;
	
	// Abstract functions meant to be implemented by deriving Resource Components
	virtual void ResourceStateChanged(ECL_ResourceState OldState, ECL_ResourceState NewState) {};

private:
	ECL_ResourceState EvaluateResourceState(float Value) const;

	FGameplayTag GetResourceGameplayTag(ECL_ResourceState State) const;
};
