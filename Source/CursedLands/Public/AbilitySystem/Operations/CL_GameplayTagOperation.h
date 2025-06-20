// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_GASOperationBase.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "CL_GameplayTagOperation.generated.h"

class UAbilitySystemComponent;

UENUM()
enum class ECL_GameplayTagOperationType : uint8
{
	Set		UMETA(DisplayName = "Set", Description = "Sets Gameplay Tag to Character"),
	Remove	UMETA(DisplayName = "Remove", Description = "Removes Gameplay Tag from Character"),
};

UCLASS(Blueprintable, EditInlineNew, Meta = (DisplayName = "Gameplay Tag Operation"))
class CURSEDLANDS_API UCL_GameplayTagOperation : public UCL_GASOperationBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "", Meta = (AllowPrivateAccess = "true"))
	FGameplayTag GameplayTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "", Meta = (AllowPrivateAccess = "true"))
	ECL_GameplayTagOperationType OperationType = ECL_GameplayTagOperationType::Set;

	//~ UCL_GASOperationBase Begin
public:
	virtual void Execute_Implementation(UAbilitySystemComponent* InAbilitySystemComponent) const override;
	virtual FString GetOperationName_Implementation() const override;
	//~ UCL_GASOperationBase End
};
