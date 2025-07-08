// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CL_AnimNotifyState_CharacterCapsuleLerp.generated.h"

UCLASS()
class CURSEDLANDS_API UCL_AnimNotifyState_CharacterCapsuleLerp : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", Meta = (AllowPrivateAccess = "true"))
	bool bLerpHalfHeight = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", Meta = (AllowPrivateAccess = "true", EditCondition = "bLerpHalfHeight", EditConditionHides))
	float NewHalfHeight = 40.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", Meta = (AllowPrivateAccess = "true"))
	bool bLerpRadius = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", Meta = (AllowPrivateAccess = "true", EditCondition = "bLerpRadius", EditConditionHides))
	float NewRadius = 40.f;
	
	float CurrentLerpDuration = 0.f;
	float TotalLerpDuration = 0.f;
	
	float InitialHalfHeight = 0.f;
	float InitialRadius = 0.f;

	bool ShouldNotify() const;
	
	//~ UAnimNotifyState Begin
public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	//~ UAnimNotifyState End
};
