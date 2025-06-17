// Copyright Anton Vasserman, All Rights Reserved.

#include "AbilitySystem/Components/CLHealthComponent.h"

#include "AbilitySystem/Attributes/CLHealthAttributeSet.h"

UCLHealthComponent::UCLHealthComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	ResourceAttributeSetClass = UCLHealthAttributeSet::StaticClass();
}
