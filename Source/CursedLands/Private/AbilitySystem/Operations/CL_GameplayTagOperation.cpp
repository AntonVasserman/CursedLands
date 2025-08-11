// Copyright Anton Vasserman, All Rights Reserved.

#include "AbilitySystem/Operations/CL_GameplayTagOperation.h"

#include "AbilitySystemComponent.h"

void UCL_GameplayTagOperation::Execute_Implementation(UAbilitySystemComponent* InAbilitySystemComponent) const
{
	checkf(InAbilitySystemComponent, TEXT("%s: AbilitySystemComponent uninitialized"), __FUNCTIONW__);
	
	switch (OperationType)
	{
	case ECL_GameplayTagOperationType::Set:
		InAbilitySystemComponent->SetLooseGameplayTagCount(GameplayTag, 1);
		break;
	case ECL_GameplayTagOperationType::Remove:
		InAbilitySystemComponent->RemoveLooseGameplayTag(GameplayTag);
		break;
	default:
		checkNoEntry();
		break;
	}
}

FString UCL_GameplayTagOperation::GetOperationName_Implementation() const
{
	return FString::Printf(TEXT("%s Gameplay Tag: %s"), *StaticEnum<ECL_GameplayTagOperationType>()->GetNameStringByValue(static_cast<int64>(OperationType)), *GameplayTag.ToString());
}
