// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CL_CommonActivatableWidget.generated.h"

UENUM(BlueprintType)
enum class ECL_WidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

UCLASS(Abstract, Blueprintable)
class CURSEDLANDS_API UCL_CommonActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UCL_CommonActivatableWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	ECL_WidgetInputMode InputConfig = ECL_WidgetInputMode::Default;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
	
	//~ Begin UCommonActivatableWidget
public:
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
#if WITH_EDITOR
	virtual void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) const override;
#endif
	
	//~ End UCommonActivatableWidget
};
