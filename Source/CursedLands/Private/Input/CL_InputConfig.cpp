// Copyright Anton Vasserman, All Rights Reserved.


#include "Input/CL_InputConfig.h"

#include "CL_LogChannels.h"
#include "GameplayTagContainer.h"

const UInputAction* UCL_InputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	if (const TObjectPtr<UInputAction>* NativeInputAction = NativeInputActionsByTag.Find(InputTag);
		NativeInputAction != nullptr)
	{
		return *NativeInputAction;
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogCL, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}
	
	return nullptr;
}

const UInputAction* UCL_InputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	if (const TObjectPtr<UInputAction>* AbilityInputAction = AbilityInputActionsByTag.Find(InputTag);
		AbilityInputAction != nullptr)
	{
		return *AbilityInputAction;
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogCL, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}
	
	return nullptr;
}
