// Copyright Anton Vasserman, All Rights Reserved.


#include "Core/HUDs/CLHUD.h"

#include "Core/UI/CLUserWidget.h"

void ACLHUD::InitOverlay()
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized in object: %s"), *GetFullName());
	
	OverlayWidget = CreateWidget<UCLUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->AddToViewport();
}

//~ AHUD Begin
//~ AHUD End
