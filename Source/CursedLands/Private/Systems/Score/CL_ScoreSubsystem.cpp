// Copyright Anton Vasserman, All Rights Reserved.


#include "Systems/Score/CL_ScoreSubsystem.h"

#include "CL_LogChannels.h"

int32 UCL_ScoreSubsystem::GetScore() const
{
	// Clamp to maximum int32 value to prevent overflow
	return static_cast<int32>(FMath::Min(CurrentScore, static_cast<uint32>(INT32_MAX)));
}

void UCL_ScoreSubsystem::AddScore(const int32 InScoreToAdd)
{
	if (InScoreToAdd < 0)
	{
		CL_LOG(Warning, TEXT("Cannot add negative score: %d"), InScoreToAdd);
		return;
	}
    
	// Check for overflow before adding
	const uint32 MaxAddable = UINT32_MAX - CurrentScore;
	if (static_cast<uint32>(InScoreToAdd) > MaxAddable)
	{
		CurrentScore = UINT32_MAX; // Cap at maximum
		CL_LOG(Warning, TEXT("Score capped at maximum value"));
	}
	else
	{
		CurrentScore += static_cast<uint32>(InScoreToAdd);
	}
}
