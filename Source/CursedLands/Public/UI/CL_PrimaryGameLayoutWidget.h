// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CL_PrimaryGameLayoutWidget.generated.h"

class UCommonActivatableWidget;

UCLASS(Abstract/*, Meta = (DisableNativeTick)*/)
class CURSEDLANDS_API UCL_PrimaryGameLayoutWidget : public UCL_CommonUserWidget
{
	GENERATED_BODY()

public:
	static UCL_PrimaryGameLayoutWidget* GetPrimaryGameLayoutForPrimaryPlayer(const UObject* WorldContextObject);
	static UCL_PrimaryGameLayoutWidget* GetPrimaryGameLayout(APlayerController* PlayerController);
	
	UCL_PrimaryGameLayoutWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) { }
	
	UFUNCTION(BlueprintCallable, Category = "UI|Layer")
	UCommonActivatableWidget* PushWidgetToLayerStack(UPARAM(Meta = (Categories = "UI.Layer")) FGameplayTag LayerTag, TSubclassOf<UCommonActivatableWidget> ActivatableWidgetClass);

	UFUNCTION(BlueprintCallable, Category = "UI|Layer")
	UCommonActivatableWidgetContainerBase* GetLayerWidget(UPARAM(Meta = (Categories = "UI.Layer")) FGameplayTag LayerTag) const;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Layer")
	void RegisterLayer(UPARAM(Meta = (Categories = "UI.Layer")) FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget);
	
private:
	UPROPERTY(Meta = (Categories = "UI.Layer"))
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;
};
