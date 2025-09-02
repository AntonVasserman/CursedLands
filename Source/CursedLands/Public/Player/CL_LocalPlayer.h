// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "CL_LocalPlayer.generated.h"

class UCL_GameLocalUserSettings;

UCLASS()
class CURSEDLANDS_API UCL_LocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()

public:
	UCL_LocalPlayer();
	
	UFUNCTION()
	UCL_GameLocalUserSettings* GetLocalSettings() const;
};
