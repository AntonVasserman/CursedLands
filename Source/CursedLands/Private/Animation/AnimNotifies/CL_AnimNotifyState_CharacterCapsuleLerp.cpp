// Copyright Anton Vasserman, All Rights Reserved.


#include "Animation/AnimNotifies/CL_AnimNotifyState_CharacterCapsuleLerp.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

bool UCL_AnimNotifyState_CharacterCapsuleLerp::ShouldNotify() const
{
	if (bLerpHalfHeight == false && bLerpRadius == false)
	{
		return false;
	}

	return true;
}

FString UCL_AnimNotifyState_CharacterCapsuleLerp::GetNotifyName_Implementation() const
{
	FString NotifySuffix = TEXT("None");
	
	if (bLerpHalfHeight && bLerpRadius)
	{
		NotifySuffix = TEXT("HalfHeight | Radius");
	}
	else if (bLerpHalfHeight)
	{
		NotifySuffix = TEXT("HalfHeight");
	}
	else if (bLerpRadius)
	{
		NotifySuffix = TEXT("Radius");
	}

	return FString::Printf(TEXT("CharacterCapsuleLerp: %s"), *NotifySuffix);
}

void UCL_AnimNotifyState_CharacterCapsuleLerp::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ShouldNotify() == false)
	{
		return;
	}
	
	const ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		return;
	}

	TotalLerpDuration = TotalDuration;
	ElapsedTime = 0.f;

	if (bLerpHalfHeight)
	{
		InitialHalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	}

	if (bLerpRadius)
	{
		InitialRadius = Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	}
}

void UCL_AnimNotifyState_CharacterCapsuleLerp::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	if (ShouldNotify() == false)
	{
		return;
	}

	const ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		return;
	}

	ElapsedTime = FMath::Min(ElapsedTime + FrameDeltaTime, TotalLerpDuration);
	const float LerpAlpha = ElapsedTime / TotalLerpDuration;
	const float NextHalfHeight = FMath::Lerp(InitialHalfHeight, NewHalfHeight, LerpAlpha);
	const float NextRadius = FMath::Lerp(InitialRadius, NewRadius, LerpAlpha);
	
	if (bLerpHalfHeight)
	{
		Character->GetCapsuleComponent()->SetCapsuleHalfHeight(NextHalfHeight);
	}
	
	if (bLerpRadius)
	{
		Character->GetCapsuleComponent()->SetCapsuleRadius(NextRadius);
	}
}

void UCL_AnimNotifyState_CharacterCapsuleLerp::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ShouldNotify() == false)
	{
		return;
	}
	
	const ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		return;
	}

	InitialHalfHeight = 0.f;
	InitialRadius = 0.f;
	ElapsedTime = 0.f;
	TotalLerpDuration = 0.f;
}
