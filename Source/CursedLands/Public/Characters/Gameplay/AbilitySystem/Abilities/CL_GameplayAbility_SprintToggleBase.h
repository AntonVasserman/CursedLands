// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_GameplayAbility_ToggleBase.h"
#include "CL_GameplayAbility_SprintToggleBase.generated.h"

UCLASS(Abstract)
class CURSEDLANDS_API UCL_GameplayAbility_SprintToggleBase : public UCL_GameplayAbility_ToggleBase
{
	GENERATED_BODY()

public:
	UCL_GameplayAbility_SprintToggleBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) { }

	//~ Begin UCL_GameplayAbility_ToggleBase
	virtual bool IsToggled(const ACL_PlayerCharacter* PlayerCharacter) const override;
	virtual bool CanToggle(const ACL_PlayerCharacter* PlayerCharacter) const override;
	virtual bool CanUnToggle(const ACL_PlayerCharacter* PlayerCharacter) const override;
	virtual void Toggle(ACL_PlayerCharacter* PlayerCharacter) override;
	virtual void UnToggle(ACL_PlayerCharacter* PlayerCharacter) override;
	//~ End UCL_GameplayAbility_ToggleBase
};
