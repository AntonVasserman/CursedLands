// Copyright Anton Vasserman, All Rights Reserved.


#include "UI/HUD/CL_HUD.h"

#include "UI/CL_UserWidget.h"

void ACL_HUD::InitOverlay()
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized in object: %s"), *GetFullName());
	
	OverlayWidget = CreateWidget<UCL_UserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->AddToViewport();
}

//~ AHUD Begin
//~ AHUD End
