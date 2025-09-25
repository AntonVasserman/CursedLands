// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CL_ProjectileBase.generated.h"

class UCapsuleComponent;
class UGameplayEffect;
class UNiagaraSystem;
class UProjectileMovementComponent;

UCLASS()
class CURSEDLANDS_API ACL_ProjectileBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> Collision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

public:
	ACL_ProjectileBase();

protected:
	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Config|Projectile")
	float ProjectileSpeed = 2500.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Gameplay", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config|VFX")
	TArray<TObjectPtr<UNiagaraSystem>> ImpactEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Config|SFX")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Config|SFX")
	TObjectPtr<USoundBase> LoopingSound;

	//~ AActor Begin
public:
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
protected:
	virtual void BeginPlay() override;
	//~ AActor End
};
