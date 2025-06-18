// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/CL_UserWidget.h"
#include "CL_AttributeProgressBar.generated.h"

class UCL_AttributeSet;
class UCL_AbilitySystemComponent;

UCLASS()
class CURSEDLANDS_API UCL_AttributeProgressBar : public UCL_UserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Ability System", meta=(ExposeOnSpawn="true"))
	FGameplayTag CurrentValueAttributeGameplayTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Ability System", meta=(ExposeOnSpawn="true"))
	FGameplayTag MaxValueAttributeGameplayTag;

	UFUNCTION(BlueprintImplementableEvent)
	void AttributeCurrentValueChanged(float NewValue);

	UFUNCTION(BlueprintImplementableEvent)
	void AttributeMaxValueChanged(float NewValue);

private:
	const UCL_AttributeSet* GetAttributeSetByGameplayTag(const UCL_AbilitySystemComponent* PlayerCharacterAbilitySystemComponent, const FGameplayTag& GameplayTag);
	
	//~ UCLUserWidget Begin
protected:
	virtual void NativeConstruct() override;
	//~ UCLUserWidget End
};
