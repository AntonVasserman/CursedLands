// Copyright Anton Vasserman, All Rights Reserved.


#include "Core/HUDs/CLHUD.h"

#include "UI/HUD/CLHUDOverlayWidget.h"

void ACLHUD::InitOverlay(UCLAbilitySystemComponent* AbilitySystem, UCLAttributeSet* AttributeSet)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized in object: %s"), *GetFullName());

	OverlayWidget = CreateWidget<UCLHUDOverlayWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->SetWidgetProperties(AbilitySystem, AttributeSet);
	OverlayWidget->BindCallbackToDependencies();
	OverlayWidget->BroadcastInitialValues();
	OverlayWidget->AddToViewport();
}

//~ AHUD Begin
//~ AHUD End
