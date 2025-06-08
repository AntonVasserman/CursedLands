// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Components/CLManaComponent.h"

#include "AbilitySystem/Attributes/CLManaAttributeSet.h"

UCLManaComponent::UCLManaComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	ResourceAttributeSetClass = UCLManaAttributeSet::StaticClass();
}
