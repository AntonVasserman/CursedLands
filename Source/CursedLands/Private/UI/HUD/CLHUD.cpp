// Copyright Anton Vasserman, All Rights Reserved.


#include "UI/HUD/CLHUD.h"

#include "UI/CLUserWidget.h"

void ACLHUD::InitOverlay()
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized in object: %s"), *GetFullName());
	
	OverlayWidget = CreateWidget<UCLUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->AddToViewport();
}

//~ AHUD Begin
//~ AHUD End
