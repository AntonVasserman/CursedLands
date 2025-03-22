// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CLHUD.generated.h"

class UCLAttributeSet;
class UCLAbilitySystemComponent;
class UCLUserWidget;

UCLASS()
class CURSEDLANDS_API ACLHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitOverlay();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Config | Overlay")
	TSubclassOf<UCLUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UCLUserWidget> OverlayWidget;

	//~ AHUD Begin
	//~ AHUD End
};
