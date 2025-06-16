// Copyright Anton Vasserman, All Rights Reserved.


#include "Systems/Falling/Tasks/CLPlayAnimationMontageLandedTask.h"

#include "GameFramework/Character.h"

//~ UCLLandedTaskBase Begin

void UCLPlayAnimationMontageLandedTask::ExecuteTask_Implementation(const FCLLandedTaskContext& TaskContext) const
{
	checkf(TaskContext.Character, TEXT("TaskContext.Character uninitialized in ApplyGameplayEffectLandedTask"));

	UAnimInstance* AnimInstance = TaskContext.Character->GetMesh()->GetAnimInstance();

	// If the user creates a task, he must assign a the anim montage to play
	checkf(AnimMontage, TEXT("AnimMontage uninitialized in PlayAnimationMontageLandedTask for Character: %s"), *TaskContext.Character->GetFullName());
	
	AnimInstance->Montage_Play(AnimMontage);
	if (bJumpToSection)
	{
		AnimInstance->Montage_JumpToSection(SectionName, AnimMontage);
	}
}

//~ UCLLandedTaskBase End
