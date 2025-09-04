// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "CL_CommonTabListWithButtonsWidgetBase.generated.h"

UCLASS(Abstract, Blueprintable, ClassGroup = UI, Meta = (DisableNativeTick))
class CURSEDLANDS_API UCL_CommonTabListWithButtonsWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> PreviousTabButton;
	FDelegateHandle PreviousTabButtonOnClickedHandle;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> NextTabButton;
	FDelegateHandle NextTabButtonOnClickedHandle;

	UFUNCTION()
	void PreviousTabButton_OnClicked();
	UFUNCTION()
	void NextTabButton_OnClicked();
	
	//~ Begin UCommonTabListWidgetBase
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~ End UCommonTabListWidgetBase
};
