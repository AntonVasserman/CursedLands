// Copyright Anton Vasserman, All Rights Reserved.


#include "UI/CL_PrimaryGameLayoutWidget.h"

#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/CL_HUD.h"

UCL_PrimaryGameLayoutWidget* UCL_PrimaryGameLayoutWidget::GetPrimaryGameLayoutForPrimaryPlayer(const UObject* WorldContextObject)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	APlayerController* PlayerController = GameInstance->GetPrimaryPlayerController(false);
	return GetPrimaryGameLayout(PlayerController);
}

UCL_PrimaryGameLayoutWidget* UCL_PrimaryGameLayoutWidget::GetPrimaryGameLayout(APlayerController* PlayerController)
{
	const ACL_HUD* CLHUD = CastChecked<ACL_HUD>(PlayerController->GetHUD());
	check(CLHUD);
	return CLHUD->GetPrimaryGameLayout();
}

UCommonActivatableWidget* UCL_PrimaryGameLayoutWidget::PushWidgetToLayerStack(FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> ActivatableWidgetClass)
{
	checkf(ActivatableWidgetClass, TEXT("%s: ActivatableWidgetClass uninitialized"), __FUNCTIONW__);
	
	if (UCommonActivatableWidgetContainerBase* Layer = GetLayerWidget(LayerName))
	{
		return Layer->AddWidget(ActivatableWidgetClass);
	}

	return nullptr;
}

UCommonActivatableWidgetContainerBase* UCL_PrimaryGameLayoutWidget::GetLayerWidget(FGameplayTag LayerName) const
{
	return Layers.FindRef(LayerName);
}

void UCL_PrimaryGameLayoutWidget::RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget)
{
	Layers.Add(LayerTag, LayerWidget);
}
