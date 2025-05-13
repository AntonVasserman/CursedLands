// Copyright Anton Vasserman, All Rights Reserved.


#include "Animation/AnimNotifies/CL_AnimNotifyState_BlendOut.h"

#include "Characters/CLCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// TODO (CL-133): Support multiple conditions
bool UCL_AnimNotifyState_BlendOut::ShouldBlendOut(const ACLCharacter* InCharacter, const ECL_BlendOutCondition InBlendOutCondition)
{
	switch (InBlendOutCondition)
	{
	case ECL_BlendOutCondition::Always:
		return true;
	case ECL_BlendOutCondition::IfMoving:
		return UKismetMathLibrary::Vector_IsNearlyZero(InCharacter->GetCharacterMovement()->GetCurrentAcceleration(), 0.1f) == false;
	case ECL_BlendOutCondition::IfFalling:
		return InCharacter->GetCharacterMovement()->IsFalling();
	case ECL_BlendOutCondition::IfFallingOrFlying:
		return InCharacter->GetCharacterMovement()->IsFalling() || InCharacter->GetCharacterMovement()->MovementMode == MOVE_Flying;
	default:
		checkNoEntry();
		return false;
	}
}

//~ UAnimNotifyState Begin

FString UCL_AnimNotifyState_BlendOut::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("BlendOut: %s"), *StaticEnum<ECL_BlendOutCondition>()->GetNameStringByValue(static_cast<int64>(BlendOutCondition)));
}

void UCL_AnimNotifyState_BlendOut::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	const ACLCharacter* Character = Cast<ACLCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		return;
	}

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	const UAnimMontage* AnimMontage = Cast<UAnimMontage>(Animation);

	if (AnimMontage == nullptr)
	{
		return;
	}

	if (ShouldBlendOut(Character, BlendOutCondition) == false)
	{
		return;
	}

	FAlphaBlendArgs AlphaBlendArgs;
	AlphaBlendArgs.BlendTime = BlendOutTime;
	AlphaBlendArgs.BlendOption = BlendOption;
	
	FMontageBlendSettings BlendOutSettings(AlphaBlendArgs);
	BlendOutSettings.BlendProfile = AnimInstance->CurrentSkeleton->GetBlendProfile(BlendProfileName);
	BlendOutSettings.BlendMode = BlendMode;
	
	AnimInstance->Montage_StopWithBlendSettings(BlendOutSettings, AnimMontage);
}

//~ UAnimNotifyState End
