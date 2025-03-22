// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CLCharacter.h"
#include "CLPlayerCharacter.generated.h"

class UGameplayCameraComponent;

UCLASS()
class CURSEDLANDS_API ACLPlayerCharacter : public ACLCharacter
{
	GENERATED_BODY()

public:
	ACLPlayerCharacter();
	
	FORCEINLINE UGameplayCameraComponent* GetGameplayCamera() const { return GameplayCamera; }

	bool CanSprint() const;
	void ToggleSprint();
	void UnToggleSprint();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config | Gameplay Camera System", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGameplayCameraComponent> GameplayCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Locomotion", meta = (AllowPrivateAccess = "true"))
	float VelocityForMinFallDamage = 1400.f;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Locomotion", meta = (AllowPrivateAccess = "true"))
	float VelocityForMaxFallDamage = 2200.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Locomotion | Gameplay Ability System", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> FallDamageGameplayEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Locomotion", meta = (AllowPrivateAccess = "true"))
	float MinWalkSpeed = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Locomotion", meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Locomotion", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Locomotion | Gameplay Ability System", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> FatigueGameplayEffectClass;

	void ApplyFatigue();
	
	//~ ACLCharacter Begin
public:
	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void Tick(float DeltaSeconds) override;
	// ACLCharacter End
	
};
