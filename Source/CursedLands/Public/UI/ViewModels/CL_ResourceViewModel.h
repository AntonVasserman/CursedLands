// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "CL_ResourceViewModel.generated.h"

UCLASS()
class CURSEDLANDS_API UCL_ResourceViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetCurrentValue() const;

	UFUNCTION(BlueprintCallable)
	void SetCurrentValue(float NewCurrentValue);

	UFUNCTION(BlueprintPure, FieldNotify)
	float GetMaxValue() const;

	UFUNCTION(BlueprintCallable)
	void SetMaxValue(float NewMaxValue);

	UFUNCTION(BlueprintPure, FieldNotify)
	float GetValuePercentage() const;

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter = "GetCurrentValue", Setter = "SetCurrentValue", Meta = (AllowPrivateAccess = "true"))
	float CurrentValue = 0.f;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter = "GetMaxValue", Setter = "SetCurrentValue", Meta = (AllowPrivateAccess = "true"))
	float MaxValue = 0.f;
};
