// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/CL_CommonActivatableWidget.h"
#include "CL_HUD.generated.h"

class UCL_CommonActivatableWidget;
class UCL_PrimaryGameLayoutWidget;
class UCL_UserWidget;

UCLASS()
class CURSEDLANDS_API ACL_HUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitPrimaryGameLayout();
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	FORCEINLINE UCL_PrimaryGameLayoutWidget* GetPrimaryGameLayout() const { return PrimaryOverlayWidget; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	FORCEINLINE UClass* GetPauseMenuWidgetClass() const { return PauseMenuWidgetClass != nullptr ? PauseMenuWidgetClass.LoadSynchronous() : nullptr; }
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Config|UI|PrimaryGameLayout")
	TSoftClassPtr<UCL_PrimaryGameLayoutWidget> PrimaryOverlayWidgetClass;
	UPROPERTY()
	TObjectPtr<UCL_PrimaryGameLayoutWidget> PrimaryOverlayWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Config|UI")
	TSoftClassPtr<UCL_CommonActivatableWidget> GameHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Config|UI")
	TSoftClassPtr<UCL_CommonActivatableWidget> PauseMenuWidgetClass;

	//~ AHUD Begin
	//~ AHUD End
};