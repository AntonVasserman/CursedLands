// Copyright Anton Vasserman, All Rights Reserved.


#include "UI/ViewModels/CL_ResourceViewModel.h"

#include "CL_LogChannels.h"
#include "Kismet/KismetMathLibrary.h"

float UCL_ResourceViewModel::GetCurrentValue() const
{
	return CurrentValue;
}

void UCL_ResourceViewModel::SetCurrentValue(float NewCurrentValue)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentValue, NewCurrentValue))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetValuePercentage);
	}
}

float UCL_ResourceViewModel::GetMaxValue() const
{
	return MaxValue;
}

void UCL_ResourceViewModel::SetMaxValue(float NewMaxValue)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxValue, NewMaxValue))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetValuePercentage);
	}
}

float UCL_ResourceViewModel::GetValuePercentage() const
{
	if (MaxValue == 0.f)
	{
		CL_LOG(Warning, "MaxValue is 0. Returning 0!");
	}

	return UKismetMathLibrary::SafeDivide(CurrentValue, MaxValue);
}
