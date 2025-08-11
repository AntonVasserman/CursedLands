// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_InputConfig.h"
#include "EnhancedInputComponent.h"
#include "CL_InputComponent.generated.h"

UCLASS(Config = Input)
class CURSEDLANDS_API UCL_InputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename FuncType>
	void BindNativeAction(const UCL_InputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound = true);
	
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UCL_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc);
};

template<class UserClass, typename FuncType>
void UCL_InputComponent::BindNativeAction(const UCL_InputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);
	if (const UInputAction* InputAction = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(InputAction, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UCL_InputComponent::BindAbilityActions(const UCL_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc)
{
	check(InputConfig);
	for (const TPair<FGameplayTag, TObjectPtr<UInputAction>> GameplayTagInputActionPair : InputConfig->AbilityInputActionsByTag)
	{
		if (GameplayTagInputActionPair.Key.IsValid() && GameplayTagInputActionPair.Value != nullptr)
		{
			if (PressedFunc != nullptr)
			{
				BindAction(GameplayTagInputActionPair.Value, ETriggerEvent::Started, Object, PressedFunc, GameplayTagInputActionPair.Key);
			}

			if (ReleasedFunc != nullptr)
			{
				BindAction(GameplayTagInputActionPair.Value, ETriggerEvent::Completed, Object, ReleasedFunc, GameplayTagInputActionPair.Key);
			}
		}
	}
}
