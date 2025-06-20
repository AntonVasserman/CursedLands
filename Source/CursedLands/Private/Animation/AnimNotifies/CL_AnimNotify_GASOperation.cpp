// Copyright Anton Vasserman, All Rights Reserved.


#include "Animation/AnimNotifies/CL_AnimNotify_GASOperation.h"

#include "CL_LogChannels.h"
#include "AbilitySystem/CL_GameplayAbilitySystemStatics.h"
#include "GameFramework/Character.h"

//~ UAnimNotify Begin

FString UCL_AnimNotify_GASOperation::GetNotifyName_Implementation() const
{
	if (GASOperation == nullptr)
	{
		return Super::GetNotifyName_Implementation();
	}

	return FString::Printf(TEXT("Try %s"), *GASOperation->GetOperationName());
}

void UCL_AnimNotify_GASOperation::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	const ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogCL, Warning, TEXT("MeshComp's Owner: %s, in AnimNotify_GameplayTagOperation is not of type ACharacter"), *Character->GetFullName());
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = UCL_GameplayAbilitySystemStatics::GetAbilitySystemComponent(Character);

	// If we can't find the Component we won't throw, we will just not apply the effect
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogCL, Warning, TEXT("Couldn't find AbilitySystemComponent on for Character: %s, in ApplyGameplayEffectLandedTask"), *Character->GetFullName());
		return;
	}

	// We don't want to check the Operation and crash in the Editor, only while in-game
	#ifdef WITH_EDITOR
	if (GASOperation == nullptr)
	{
		UE_LOG(LogCL, Warning, TEXT("Operation uninitialized in AnimNotify_GameplayTagOperation for Character: %s"), *Character->GetFullName());
		return;
	}
	#else
	checkf(Operation, TEXT("Operation uninitialized in AnimNotify_GameplayTagOperation for Character: %s"), *TaskContext.Character->GetFullName());	
	#endif
	
	GASOperation->Execute(AbilitySystemComponent);
}

//~ UAnimNotify End
