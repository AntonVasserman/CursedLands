// Copyright Anton Vasserman, All Rights Reserved.


#include "Animation/AnimNotifies/CLAnimNotifyState_ForceBlendOut.h"

#include "Characters/CLCharacter.h"

//~ UAnimNotifyState Begin

void UCLAnimNotifyState_ForceBlendOut::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (Cast<ACLCharacter>(MeshComp->GetOwner()) == nullptr)
	{
		return;
	}

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	UAnimMontage* AnimMontage = Cast<UAnimMontage>(Animation);

	if (AnimMontage == nullptr)
	{
		return;
	}

	FMontageBlendSettings BlendOutSettings;
	UBlendProfile* Profile = AnimInstance->CurrentSkeleton->GetBlendProfile(BlendProfileName);
	BlendOutSettings.BlendProfile = Profile;

	FAlphaBlendArgs AlphaBlendArgs;
	AlphaBlendArgs.BlendTime = BlendOutTime;
	AlphaBlendArgs.BlendOption = EAlphaBlendOption::HermiteCubic;
	BlendOutSettings.Blend = AlphaBlendArgs;
	
	BlendOutSettings.BlendMode = BlendMode;
	
	AnimInstance->Montage_StopWithBlendSettings(BlendOutSettings, AnimMontage);
}

//~ UAnimNotifyState End
