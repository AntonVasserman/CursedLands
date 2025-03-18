// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/UI/CLUserWidget.h"
#include "CLHUDOverlayWidget.generated.h"

class UCLAbilitySystemComponent;
class UCLAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

UCLASS()
class CURSEDLANDS_API UCLHUDOverlayWidget : public UCLUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void HealthChanged(float NewHealthValue);

	UFUNCTION(BlueprintImplementableEvent)
	void MaxHealthChanged(float NewMaxHealthValue);

	UFUNCTION(BlueprintImplementableEvent)
	void StaminaChanged(float NewStaminaValue);

	UFUNCTION(BlueprintImplementableEvent)
	void MaxStaminaChanged(float NewMaxStaminaValue);
	
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();
	virtual void BindCallbackToDependencies();
	virtual void SetWidgetProperties(UCLAbilitySystemComponent* AbilitySystem, UCLAttributeSet* AttributeSet);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System")
	TObjectPtr<UCLAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System")
	TObjectPtr<UCLAttributeSet> AttributeSet;

	//~ UCLUserWidget Begin
	//~ UCLUserWidget End
};
