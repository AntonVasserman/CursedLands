// Copyright Anton Vasserman, All Rights Reserved.

#include "Components/Models/AV_FallRange.h"

bool FAV_FallRange::TestRange(float FallHeight) const
{
	switch (ThresholdType)
	{
	case EAV_FallRangeThresholdType::LessThan:
		return FallHeight < FallHeightThreshold;
	case EAV_FallRangeThresholdType::InRange:
		return FallHeight >= FallHeightThresholdMin && FallHeight <= FallHeightThresholdMax;
	case EAV_FallRangeThresholdType::GreaterThan:
		return FallHeight > FallHeightThreshold;
	default:
		checkNoEntry();
		return false;
	}
}
