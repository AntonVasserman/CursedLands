// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Operations/CL_GameplayTagOperation.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CL_AnimNotify_GASOperation.generated.h"

UCLASS()
class CURSEDLANDS_API UCL_AnimNotify_GASOperation : public UAnimNotify
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Instanced, Category = "")
	TObjectPtr<UCL_GASOperationBase> GASOperation = nullptr;

	//~ UAnimNotify Begin
public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	//~ UAnimNotify End
};
