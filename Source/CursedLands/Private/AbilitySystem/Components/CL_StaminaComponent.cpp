// Copyright Anton Vasserman, All Rights Reserved.

#include "AbilitySystem/Components/CL_StaminaComponent.h"

#include "AbilitySystem/Attributes/CL_StaminaAttributeSet.h"

UCL_StaminaComponent::UCL_StaminaComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	ResourceAttributeSetClass = UCL_StaminaAttributeSet::StaticClass();
}
