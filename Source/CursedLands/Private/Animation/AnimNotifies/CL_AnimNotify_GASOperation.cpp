// Copyright Anton Vasserman, All Rights Reserved.


#include "Animation/AnimNotifies/CL_AnimNotify_GASOperation.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CL_LogChannels.h"
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
	
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		CL_LOG(Warning, "MeshComp's Owner: '%s' is not of type ACharacter", *Character->GetFullName());
		return;
	}

	// UAbilitySystemBlueprintLibrary::UAbilitySystemBlueprintLibrary
	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character);

	// If we can't find the Component we won't throw, we will just not apply the effect
	if (AbilitySystemComponent == nullptr)
	{
		CL_LOG(Warning, "Couldn't find AbilitySystemComponent on for Character: %s", *Character->GetFullName());
		return;
	}

	// Since this is an anim notify, and will be constantly executed when created,
	// we don't want to check the Operation and crash the Editor, we only want to do it while in-game/PIE.
	#ifdef WITH_EDITOR
	if (GASOperation == nullptr)
	{
		CL_LOG(Warning, "GASOperation uninitialized for Character: %s", *Character->GetFullName());
		return;
	}
	#else
	checkf(GASOperation, TEXT("%s: GASOperation uninitialized for Character: %s"), __FUNCTIONW__, *Character->GetFullName());
	#endif
	
	GASOperation->Execute(AbilitySystemComponent);
}

//~ UAnimNotify End
