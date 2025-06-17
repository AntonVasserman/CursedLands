// Copyright Anton Vasserman, All Rights Reserved.


#include "Animation/AnimNotifies/CL_AnimNotify_GameplayTagOperation.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "CLLogChannels.h"
#include "AbilitySystem/CL_GameplayAbilitySystemStatics.h"
#include "GameFramework/Character.h"

//~ UAnimNotify Begin

FString UCL_AnimNotify_GameplayTagOperation::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("Try %s Gameplay Tag: %s"), *StaticEnum<ECL_GameplayTagOperationType>()->GetNameStringByValue(static_cast<int64>(OperationType)), *GameplayTag.ToString());
}

void UCL_AnimNotify_GameplayTagOperation::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	const ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogCL, Warning, TEXT("MeshComp's Owner: %s, in AnimNotify_GameplayTagOperation is not of type ACharacter"), *Character->GetFullName());
		return;
	}

	TOptional<UAbilitySystemComponent*> AbilitySystemComponent = UCL_GameplayAbilitySystemStatics::GetAbilitySystemComponent(Character);

	// If we can't find the Component we won't throw, we will just not apply the effect
	if (AbilitySystemComponent.IsSet() == false)
	{
		UE_LOG(LogCL, Warning, TEXT("Couldn't find AbilitySystemComponent on for Character: %s, in ApplyGameplayEffectLandedTask"), *Character->GetFullName());
		return;
	}

	switch (OperationType)
	{
	case ECL_GameplayTagOperationType::Set:
		AbilitySystemComponent.GetValue()->SetLooseGameplayTagCount(GameplayTag, 1);
		break;
	case ECL_GameplayTagOperationType::Remove:
		AbilitySystemComponent.GetValue()->RemoveLooseGameplayTag(GameplayTag);
		break;
	default:
		checkNoEntry();
		break;
	}
}

//~ UAnimNotify End
