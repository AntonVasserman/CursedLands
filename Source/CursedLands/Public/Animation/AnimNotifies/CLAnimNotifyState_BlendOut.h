// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLBlendOutCondition.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Characters/CLCharacter.h"
#include "CLAnimNotifyState_BlendOut.generated.h"

UCLASS()
class CURSEDLANDS_API UCL_AnimNotifyState_BlendOut : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config|Animation", Meta = (AllowPrivateAccess = "true"))
	ECL_BlendOutCondition BlendOutCondition = ECL_BlendOutCondition::Always;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config|Animation", Meta = (AllowPrivateAccess = "true"))
	FName BlendProfileName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config|Animation", Meta = (AllowPrivateAccess = "true"))
	float BlendOutTime = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config|Animation", Meta = (AllowPrivateAccess = "true"))
	EAlphaBlendOption BlendOption = EAlphaBlendOption::HermiteCubic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config|Animation", Meta = (AllowPrivateAccess = "true"))
	EMontageBlendMode BlendMode = EMontageBlendMode::Inertialization;

	static bool ShouldBlendOut(const ACLCharacter* InCharacter, const ECL_BlendOutCondition InBlendOutCondition);
	
	//~ UAnimNotifyState Begin
public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	//~ UAnimNotifyState End
};
