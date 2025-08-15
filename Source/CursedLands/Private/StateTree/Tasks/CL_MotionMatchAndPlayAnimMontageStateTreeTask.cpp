// Copyright Anton Vasserman, All Rights Reserved.


#include "StateTree/Tasks/CL_MotionMatchAndPlayAnimMontageStateTreeTask.h"

#include "CL_LogChannels.h"
#include "StateTreeExecutionContext.h"
#include "PoseSearch/PoseSearchLibrary.h"
#include "GameFramework/Character.h"

FCL_MotionMatchAndPlayAnimMontageStateTreeTask::FCL_MotionMatchAndPlayAnimMontageStateTreeTask()
{
	bShouldCallTick = false;
}

//~ Begin FStateTreeTaskCommonBase

EStateTreeRunStatus FCL_MotionMatchAndPlayAnimMontageStateTreeTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	
	if (InstanceData.AnimMontages.Num() == 0)
	{
		CL_LOG(Warning, "Empty AnimMontages provided for Character: %s", *InstanceData.Character->GetFullName());
		return EStateTreeRunStatus::Succeeded;
	}

	UAnimInstance* AnimInstance = InstanceData.Character->GetMesh()->GetAnimInstance();

	FPoseSearchBlueprintResult Result;
	UPoseSearchLibrary::MotionMatch(
		AnimInstance,
		TArray<UObject*>(InstanceData.AnimMontages),
		InstanceData.PoseHistoryName,
		FPoseSearchContinuingProperties(),
		FPoseSearchFutureProperties(),
		Result);

	UAnimMontage* AnimMontage = Cast<UAnimMontage>(Result.SelectedAnim);

	if (IsValid(AnimMontage) == false)
	{
		CL_LOG(Warning, "Couldn't find anim montage to play for Character: %s", *InstanceData.Character->GetFullName());
		return EStateTreeRunStatus::Failed;
	}
	
	AnimInstance->Montage_Play(AnimMontage, Result.WantedPlayRate, EMontagePlayReturnType::MontageLength, Result.SelectedTime);
	if (InstanceData.bJumpToSection)
	{
		AnimInstance->Montage_JumpToSection(InstanceData.SectionName, AnimMontage);
	}

	return EStateTreeRunStatus::Succeeded;
}

#if WITH_EDITOR

EDataValidationResult FCL_MotionMatchAndPlayAnimMontageStateTreeTask::Compile(UE::StateTree::ICompileNodeContext& Context)
{
	const FInstanceDataType& InstanceData = Context.GetInstanceDataView().Get<FInstanceDataType>();
	bool bValid = true;

	if (InstanceData.AnimMontages.Num() == 0)
	{
		Context.AddValidationError(FText::FromString(TEXT("No AnimMontages provided. Motion matching requires at least one animation montage.")));
		bValid = false;
	}
	else
	{
		for (UObject* AnimMontage : InstanceData.AnimMontages)
		{
			if (IsValid(AnimMontage) == false)
			{
				Context.AddValidationError(FText::FromString(TEXT("Invalid AnimMontage provided.")));
				bValid = false;
				break;
			}
		}
	}

	if (InstanceData.PoseHistoryName == NAME_None)
	{
		Context.AddValidationError(FText::FromString(TEXT("PoseHistoryName is not set. Motion matching may not work correctly without a valid pose history.")));
		bValid = false;
	}

	return bValid ? EDataValidationResult::Valid : EDataValidationResult::Invalid;
}

FText FCL_MotionMatchAndPlayAnimMontageStateTreeTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FInstanceDataType* InstanceData = InstanceDataView.GetPtr<FInstanceDataType>();
	
	if (!InstanceData)
	{
		return FText::FromString(TEXT("Motion Match and Play Anim Montage"));
	}

	FString AnimMontagesDescription = TEXT("None");
	if (InstanceData->AnimMontages.Num() > 0)
	{
		if (InstanceData->AnimMontages.Num() == 1 && IsValid(InstanceData->AnimMontages[0]))
		{
			AnimMontagesDescription = InstanceData->AnimMontages[0]->GetName();
		}
		else
		{
			AnimMontagesDescription = FString::Printf(TEXT("%d Montages"), InstanceData->AnimMontages.Num());
		}
	}

	const FString PoseHistoryString = FString::Printf(TEXT(" (History: %s)"), *InstanceData->PoseHistoryName.ToString());
	const FString JumpToSectionString = InstanceData->bJumpToSection ? FString::Printf(TEXT(" from Section: %s"), *InstanceData->SectionName.ToString()) : TEXT("");
	
	return FText::FromString(FString::Printf(TEXT("Motion Match and Play: %s%s%s"), *AnimMontagesDescription, *PoseHistoryString, *JumpToSectionString));
}
#endif

//~ End FStateTreeTaskCommonBase