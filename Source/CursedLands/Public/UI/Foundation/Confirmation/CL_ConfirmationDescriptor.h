// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_ConfirmationWidget.h"
#include "UObject/Object.h"
#include "CL_ConfirmationDescriptor.generated.h"

USTRUCT(BlueprintType)
struct FConfirmationAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECL_ConfirmationWidgetResult Result = ECL_ConfirmationWidgetResult::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText OptionalDisplayText;

	bool operator==(const FConfirmationAction& Other) const
	{
		return Result == Other.Result && OptionalDisplayText.EqualTo(Other.OptionalDisplayText);
	}
};

UCLASS()
class CURSEDLANDS_API UCL_ConfirmationDescriptor : public UObject
{
	GENERATED_BODY()

public:
	static UCL_ConfirmationDescriptor* CreateConfirmationYesNo(const FText& Header, const FText& Body);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Header;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Body;

	UPROPERTY(BlueprintReadWrite)
	TArray<FConfirmationAction> ButtonActions;
};
