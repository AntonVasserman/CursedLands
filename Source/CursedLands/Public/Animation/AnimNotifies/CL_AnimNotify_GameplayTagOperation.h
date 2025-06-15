// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CL_AnimNotify_GameplayTagOperation.generated.h"

class UAbilitySystemComponent;

UENUM()
enum class ECL_GameplayTagOperationType : uint8
{
	Set		UMETA(DisplayName = "Set", Description = "Sets Gameplay Tag to Character"),
	Remove	UMETA(DisplayName = "Remove", Description = "Removes Gameplay Tag from Character"),
};

UCLASS()
class CURSEDLANDS_API UCL_AnimNotify_GameplayTagOperation : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", Meta = (AllowPrivateAccess = "true"))
	FGameplayTag GameplayTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Config", Meta = (AllowPrivateAccess = "true"))
	ECL_GameplayTagOperationType OperationType = ECL_GameplayTagOperationType::Set;
	
	//~ UAnimNotify Begin
public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	//~ UAnimNotify End
};
