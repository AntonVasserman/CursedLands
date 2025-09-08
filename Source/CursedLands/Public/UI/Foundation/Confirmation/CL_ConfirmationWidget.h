// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UObject/Object.h"
#include "CL_ConfirmationWidget.generated.h"

class UCommonButtonBase;
class UCommonBorder;
class UCommonRichTextBlock;
class UCommonTextBlock;
class UDynamicEntryBox;
class UCL_ConfirmationDescriptor;

UENUM(BlueprintType)
enum class ECL_ConfirmationWidgetResult : uint8
{
	None = 0	UMETA(Hidden),
	Yes			UMETA(DisplayName = "Yes", Description = "The 'yes' button was pressed"),
	No			UMETA(DisplayName = "No", Description = "The 'no' button was pressed"),
};

DECLARE_DELEGATE_OneParam(FCL_ConfirmationResultDelegate, ECL_ConfirmationWidgetResult /* Result */);

UCLASS(Abstract, BlueprintType, Blueprintable)
class CURSEDLANDS_API UCL_ConfirmationWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void Setup(UCL_ConfirmationDescriptor* Descriptor, FCL_ConfirmationResultDelegate ResultDelegate);

protected:
	void CloseConfirmationWindow(ECL_ConfirmationWidgetResult Result);

private:
	UFUNCTION()
	FEventReply HandleTapToCloseZoneMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	FCL_ConfirmationResultDelegate OnResultCallback;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Text_Title;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCommonRichTextBlock> RichText_Description;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UDynamicEntryBox> EntryBox_Buttons;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCommonBorder> Border_TapToCloseZone;

	UPROPERTY(EditDefaultsOnly, Meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle CancelAction;
	
	//~ Begin UCommonActivatableWidget
public:
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif
protected:
	virtual void NativeOnInitialized() override;
	//~ End UCommonActivatableWidget
};
