// Copyright Anton Vasserman, All Rights Reserved.


#include "Tasks/Landed/AV_PlayAnimationMontageLandedTask.h"

#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"


//~ UAV_LandedTaskBase Begin

void UAV_PlayAnimationMontageLandedTask::ExecuteTaskInternal_Implementation(const FAV_LandedTaskContext& TaskContext) const
{
	checkf(TaskContext.Character, TEXT("%s::%hs: TaskContext.Character uninitialized"), *GetClass()->GetName(), __FUNCTION__);

	UAnimInstance* AnimInstance = TaskContext.Character->GetMesh()->GetAnimInstance();

	// If the user creates a task, he must assign a the anim montage to play
	checkf(AnimMontage, TEXT("%s::%hs: AnimMontage uninitialized for Character: %s"), *GetClass()->GetName(), __FUNCTION__, *TaskContext.Character->GetFullName());
	
	AnimInstance->Montage_Play(AnimMontage);
	if (bJumpToSection)
	{
		AnimInstance->Montage_JumpToSection(SectionName, AnimMontage);
	}
}

//~ UAV_LandedTaskBase End
