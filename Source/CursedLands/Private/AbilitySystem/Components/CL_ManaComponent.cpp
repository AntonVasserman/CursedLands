// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Components/CL_ManaComponent.h"

#include "AbilitySystem/Attributes/CL_ManaAttributeSet.h"

UCL_ManaComponent::UCL_ManaComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	ResourceAttributeSetClass = UCL_ManaAttributeSet::StaticClass();
}

//~ UCL_ResourceComponent Begin
//~ UCL_ResourceComponent End