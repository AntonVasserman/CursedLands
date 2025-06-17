// Copyright Anton Vasserman, All Rights Reserved.

#include "AbilitySystem/Components/CL_HealthComponent.h"

#include "AbilitySystem/Attributes/CL_HealthAttributeSet.h"

UCL_HealthComponent::UCL_HealthComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	ResourceAttributeSetClass = UCL_HealthAttributeSet::StaticClass();
}
