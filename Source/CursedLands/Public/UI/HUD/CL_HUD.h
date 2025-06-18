// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CL_HUD.generated.h"

class UCL_AttributeSet;
class UCL_AbilitySystemComponent;
class UCL_UserWidget;

UCLASS()
class CURSEDLANDS_API ACL_HUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitOverlay();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Config|Overlay")
	TSubclassOf<UCL_UserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UCL_UserWidget> OverlayWidget;

	//~ AHUD Begin
	//~ AHUD End
};
