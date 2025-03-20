// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Characters/CLCharacter.h"
#include "CLPlayerCharacter.generated.h"

class UGameplayCameraComponent;

UCLASS()
class CURSEDLANDS_API ACLPlayerCharacter : public ACLCharacter
{
	GENERATED_BODY()

public:
	ACLPlayerCharacter();
	
	FORCEINLINE UGameplayCameraComponent* GetGameplayCamera() const { return GameplayCamera; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config | Gameplay Camera System", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGameplayCameraComponent> GameplayCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Gameplay Ability System", meta = (AllowPrivateAccess = "true"))
	float VelocityForMinFallDamage = 1400.f;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Gameplay Ability System", meta = (AllowPrivateAccess = "true"))
	float VelocityForMaxFallDamage = 2200.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config | Gameplay Ability System", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> FallDamageGameplayEffectClass;
	
	//~ ACLCharacter Begin
	virtual void Landed(const FHitResult& Hit) override;
	// ACLCharacter End
};
