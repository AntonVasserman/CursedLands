// Copyright Anton Vasserman, All Rights Reserved.


#include "UI/HUD/CLHUDOverlayWidget.h"

#include "AbilitySystem/CLAbilitySystemComponent.h"
#include "AbilitySystem/CLAttributeSet.h"
#include "GameplayEffectTypes.h"


void UCLHUDOverlayWidget::BroadcastInitialValues()
{
	HealthChanged(AttributeSet->GetHealth());
	MaxHealthChanged(AttributeSet->GetMaxHealth());
	StaminaChanged(AttributeSet->GetStamina());
	MaxStaminaChanged(AttributeSet->GetMaxStamina());
}

void UCLHUDOverlayWidget::BindCallbackToDependencies()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			HealthChanged(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			MaxHealthChanged(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			StaminaChanged(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxStaminaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			MaxStaminaChanged(Data.NewValue);
		});
}

void UCLHUDOverlayWidget::SetWidgetProperties(UCLAbilitySystemComponent* InAbilitySystem, UCLAttributeSet* InAttributeSet)
{
	AbilitySystemComponent = InAbilitySystem;
	AttributeSet = InAttributeSet;
}

//~ UCLUserWidget Begin
//~ UCLUserWidget End