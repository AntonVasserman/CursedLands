// Copyright Anton Vasserman, All Rights Reserved.

#include "Characters/CLStaminaComponent.h"

#include "AbilitySystem/Attributes/CLStaminaAttributeSet.h"

UCLStaminaComponent::UCLStaminaComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	ResourceAttributeSetClass = UCLStaminaAttributeSet::StaticClass();
}
