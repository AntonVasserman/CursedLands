// Copyright Anton Vasserman, All Rights Reserved.


#include "UI/HUD/CL_HUD.h"

#include "CL_GameplayTags.h"
#include "UI/CL_CommonActivatableWidget.h"
#include "UI/CL_UserWidget.h"
#include "UI/CL_PrimaryGameLayoutWidget.h"

void ACL_HUD::InitPrimaryGameLayout()
{
	checkf(PrimaryOverlayWidgetClass, TEXT("PrimaryOverlayWidgetClass uninitialized in object: %s"), *GetFullName());
	PrimaryOverlayWidget = CreateWidget<UCL_PrimaryGameLayoutWidget>(GetWorld(), PrimaryOverlayWidgetClass);
	PrimaryOverlayWidget->AddToViewport();

	checkf(GameHUDWidgetClass, TEXT("GameHUDWidgetClass uninitialized in object: %s"), *GetFullName());
	PrimaryOverlayWidget->PushWidgetToLayerStack(CLGameplayTags::UI_Layer_Game, GameHUDWidgetClass);
}

//~ AHUD Begin
//~ AHUD End
