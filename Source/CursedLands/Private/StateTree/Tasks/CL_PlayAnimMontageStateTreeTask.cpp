// Copyright Anton Vasserman, All Rights Reserved.


#include "StateTree/Tasks/CL_PlayAnimMontageStateTreeTask.h"

#include "StateTreeExecutionContext.h"
#include "GameFramework/Character.h"

FCL_PlayAnimMontageStateTreeTask::FCL_PlayAnimMontageStateTreeTask()
{
	bShouldCallTick = false;
}

EStateTreeRunStatus FCL_PlayAnimMontageStateTreeTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	const ACharacter* Character = Cast<ACharacter>(InstanceData.Context->GetPawn());
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	
	if (!IsValid(InstanceData.AnimMontage))
	{
		return EStateTreeRunStatus::Failed;
	}
	
	AnimInstance->Montage_Play(InstanceData.AnimMontage);
	if (InstanceData.bJumpToSection)
	{
		AnimInstance->Montage_JumpToSection(InstanceData.SectionName, InstanceData.AnimMontage);
	}

	return EStateTreeRunStatus::Succeeded;
}

#if WITH_EDITOR
FText FCL_PlayAnimMontageStateTreeTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FInstanceDataType* InstanceData = InstanceDataView.GetPtr<FInstanceDataType>();
	
	if (!InstanceData)
	{
		return FText::FromString(TEXT("Play Anim Montage"));
	}

	FString AnimMontageName = TEXT("None");
	if (InstanceData->AnimMontage)
	{
		AnimMontageName = InstanceData->AnimMontage->GetName();
	}

	FString JumpToSectionString = TEXT("");
	if (InstanceData->bJumpToSection)
	{
		JumpToSectionString = FString::Printf(TEXT(" from Section: %s"), *InstanceData->SectionName.ToString());
	}
	
	return FText::FromString(FString::Printf(TEXT("Play Anim Montage: %s%s"), *AnimMontageName, *JumpToSectionString));
}
#endif
