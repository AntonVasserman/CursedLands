// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CLHUD.generated.h"

class UCLAttributeSet;
class UCLAbilitySystemComponent;
class UCLHUDOverlayWidget;

UCLASS()
class CURSEDLANDS_API ACLHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitOverlay(UCLAbilitySystemComponent* AbilitySystem, UCLAttributeSet* AttributeSet);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Config | Overlay")
	TSubclassOf<UCLHUDOverlayWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UCLHUDOverlayWidget> OverlayWidget;

	//~ AHUD Begin
	//~ AHUD End
};
