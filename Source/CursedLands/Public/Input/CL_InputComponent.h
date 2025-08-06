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
	
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UCL_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc, TArray<uint32>& BindHandles);
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

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UCL_InputComponent::BindAbilityActions(const UCL_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);
	for (const TPair<FGameplayTag, TObjectPtr<UInputAction>> GameplayTagInputActionPair : InputConfig->AbilityInputActionsByTag)
	{
		if (GameplayTagInputActionPair.Key.IsValid() && GameplayTagInputActionPair.Value != nullptr)
		{
			if (PressedFunc != nullptr)
			{
				BindHandles.Add(BindAction(GameplayTagInputActionPair.Value, ETriggerEvent::Started, Object, PressedFunc, GameplayTagInputActionPair.Key).GetHandle());
			}

			if (ReleasedFunc != nullptr)
			{
				BindHandles.Add(BindAction(GameplayTagInputActionPair.Value, ETriggerEvent::Completed, Object, ReleasedFunc, GameplayTagInputActionPair.Key).GetHandle());
			}

			if (HeldFunc != nullptr)
			{
				BindHandles.Add(BindAction(GameplayTagInputActionPair.Value, ETriggerEvent::Triggered, Object, HeldFunc, GameplayTagInputActionPair.Key).GetHandle());
			}
		}
	}
}
