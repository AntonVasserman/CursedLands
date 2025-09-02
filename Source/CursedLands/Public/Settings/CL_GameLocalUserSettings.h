// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CL_GameLocalUserSettings.generated.h"

UCLASS()
class CURSEDLANDS_API UCL_GameLocalUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	static UCL_GameLocalUserSettings* Get();
	
	// Audio
	UFUNCTION()
	float GetOverallVolume() const;
	UFUNCTION()
	void SetOverallVolume(float InVolume);

private:
	UPROPERTY(Config)
	float OverallVolume = 1.0f;
};
