// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLAnimInstance.h"
#include "Characters/CLPlayerCharacterMovementMode.h"
#include "CLPlayerCharacterAnimInstance.generated.h"

class ACLPlayerCharacter;

UCLASS()
class CURSEDLANDS_API UCLPlayerCharacterAnimInstance : public UCLAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data", Meta = (AllowPrivateAccess = "true"))
	ECLPlayerCharacterMovementMode MovementMode;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data", Meta = (AllowPrivateAccess = "true"))
	float FallHeight = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data | Constants", Meta = (AllowPrivateAccess = "true"))
	float FallHeightForMinFallDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data | Constants", Meta = (AllowPrivateAccess = "true"))
	float FallHeightForMaxFallDamage;

private:
	UPROPERTY()
	TObjectPtr<ACLPlayerCharacter> PlayerCharacter;
	
	//~ UCLAnimInstance Begin
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//~ UCLAnimInstance End
};
