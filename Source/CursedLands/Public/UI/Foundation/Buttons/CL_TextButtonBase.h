// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CL_TextButtonBase.generated.h"

class UCommonTextBlock;

UCLASS(Abstract, BlueprintType, Blueprintable)
class CURSEDLANDS_API UCL_TextButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(const FText& InText);

protected:
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Text_Button;
	
	//~ Begin UCommonButtonBase
	//~ End UCommonButtonBase
};
