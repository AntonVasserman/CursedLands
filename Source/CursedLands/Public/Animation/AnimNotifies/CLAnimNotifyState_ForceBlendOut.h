// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CLAnimNotifyState_ForceBlendOut.generated.h"

struct FMontageBlendSettings;

UCLASS()
class CURSEDLANDS_API UCLAnimNotifyState_ForceBlendOut : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config|Animation", Meta = (AllowPrivateAccess = "true"))
	FName BlendProfileName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config|Animation", Meta = (AllowPrivateAccess = "true"))
	float BlendOutTime = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config|Animation", Meta = (AllowPrivateAccess = "true"))
	EAlphaBlendOption BlendOption = EAlphaBlendOption::HermiteCubic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config|Animation", Meta = (AllowPrivateAccess = "true"))
	EMontageBlendMode BlendMode = EMontageBlendMode::Inertialization;
	
	//~ UAnimNotifyState Begin
public:
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	//~ UAnimNotifyState End
};
