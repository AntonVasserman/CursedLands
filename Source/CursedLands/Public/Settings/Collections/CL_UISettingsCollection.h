// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_SettingsCollection.h"
#include "CL_UISettingsCollection.generated.h"

UCLASS()
class CURSEDLANDS_API UCL_UISettingsCollection : public UCL_SettingsCollection
{
	GENERATED_BODY()

	//~ UCL_SettingsCollection Begin
public:
	virtual void InitializeSettings(UCL_LocalPlayer* InLocalPlayer) override;
	//~ UCL_SettingsCollection End
};
