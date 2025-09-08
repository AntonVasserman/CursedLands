// Copyright Anton Vasserman, All Rights Reserved.


#include "UI/Foundation/Confirmation/CL_ConfirmationDescriptor.h"

#define LOCTEXT_NAMESPACE "Confirmation"

UCL_ConfirmationDescriptor* UCL_ConfirmationDescriptor::CreateConfirmationYesNo(const FText& Header, const FText& Body)
{
	UCL_ConfirmationDescriptor* Descriptor = NewObject<UCL_ConfirmationDescriptor>();
	Descriptor->Header = Header;
	Descriptor->Body = Body;

	FConfirmationAction YesAction;
	YesAction.Result = ECL_ConfirmationWidgetResult::Yes;
	YesAction.OptionalDisplayText = LOCTEXT("Yes", "Yes");

	FConfirmationAction NoAction;
	NoAction.Result = ECL_ConfirmationWidgetResult::No;
	NoAction.OptionalDisplayText = LOCTEXT("No", "No");

	Descriptor->ButtonActions.Add(YesAction);
	Descriptor->ButtonActions.Add(NoAction);

	return Descriptor;
}

#undef LOCTEXT_NAMESPACE
