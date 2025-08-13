// Copyright Anton Vasserman, All Rights Reserved.


#include "UI/CL_CommonActivatableWidget.h"

#include "Editor/WidgetCompilerLog.h"

UCL_CommonActivatableWidget::UCL_CommonActivatableWidget(const FObjectInitializer& ObjectInitializer) { }

//~ Begin UCommonActivatableWidget

TOptional<FUIInputConfig> UCL_CommonActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
		case ECL_WidgetInputMode::GameAndMenu:
			return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
		case ECL_WidgetInputMode::Game:
			return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
		case ECL_WidgetInputMode::Menu:
			return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
		case ECL_WidgetInputMode::Default:
		default:
			return TOptional<FUIInputConfig>();
	}
}

#if WITH_EDITOR

void UCL_CommonActivatableWidget::ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledWidgetTree(BlueprintWidgetTree, CompileLog);

	if (!GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UCL_CommonActivatableWidget, BP_GetDesiredFocusTarget)))
	{
		CompileLog.Warning(FText::FromString(TEXT("GetDesiredFocusTarget wasn't implemented, you're going to have trouble using gamepads on this screen.")));
	}
}

#endif

//~ End UCommonActivatableWidget
