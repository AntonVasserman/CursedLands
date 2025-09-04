// Copyright Anton Vasserman, All Rights Reserved.


#include "UI/Settings/CL_CommonTabListWithButtonsWidgetBase.h"

#include "CommonButtonBase.h"

void UCL_CommonTabListWithButtonsWidgetBase::PreviousTabButton_OnClicked()
{
	bool bPassThrough;
	HandlePreviousTabInputAction(bPassThrough);
}

void UCL_CommonTabListWithButtonsWidgetBase::NextTabButton_OnClicked()
{
	bool bPassThrough;
	HandleNextTabInputAction(bPassThrough);
}

//~ Begin UCommonTabListWidgetBase

void UCL_CommonTabListWithButtonsWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	PreviousTabButton->SetTriggeredInputAction(PreviousTabInputActionData);
	NextTabButton->SetTriggeredInputAction(NextTabInputActionData);
	
	// Setup Bindings
	PreviousTabButtonOnClickedHandle = PreviousTabButton->OnClicked().AddUObject(this, &UCL_CommonTabListWithButtonsWidgetBase::PreviousTabButton_OnClicked);
	NextTabButtonOnClickedHandle = NextTabButton->OnClicked().AddUObject(this, &UCL_CommonTabListWithButtonsWidgetBase::NextTabButton_OnClicked);
}

void UCL_CommonTabListWithButtonsWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();

	// Clear Bindings
	PreviousTabButton->OnClicked().Remove(PreviousTabButtonOnClickedHandle);
	NextTabButton->OnClicked().Remove(NextTabButtonOnClickedHandle);
}

//~ End UCommonTabListWidgetBase
