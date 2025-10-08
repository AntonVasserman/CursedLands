// Copyright Anton Vasserman, All Rights Reserved.


#include "Actors/Projectiles/CL_ProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DataValidation.h"
#include "Settings/CL_GameLocalUserSettings.h"

ACL_ProjectileBase::ACL_ProjectileBase()
{
	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
	Collision->SetCollisionProfileName("Projectile");
	Collision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
	ProjectileMovement->InitialSpeed = ProjectileSpeed;
	ProjectileMovement->MaxSpeed = ProjectileSpeed;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void ACL_ProjectileBase::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetInstigator())
	{
		return;
	}

	UAbilitySystemComponent* OtherActorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (OtherActorASC == nullptr)
	{
		return;
	}

	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = OtherActorASC->MakeEffectContext();
	ContextHandle.AddSourceObject(GetInstigator());
	float GameplayEffectLevel = 1.f;
	switch (UCL_GameLocalUserSettings::Get()->GetDifficulty())
	{
	case ECL_Difficulty::Normal:
		GameplayEffectLevel = 1.f;
		break;
	case ECL_Difficulty::Hard:
		GameplayEffectLevel = 2.f;
		break;
	default:
		checkNoEntry();
	}
	const FGameplayEffectSpecHandle SpecHandle = OtherActorASC->MakeOutgoingSpec(GameplayEffectClass, GameplayEffectLevel, ContextHandle);
	OtherActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	if (ImpactEffects.Num() > 0)
	{
		if (const int32 RandomIndex = FMath::RandRange(0, ImpactEffects.Num() - 1);
			ImpactEffects[RandomIndex] != nullptr)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffects[RandomIndex], GetActorLocation());
		}
	}

	if (ImpactSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	}

	if (LoopingSound != nullptr)
	{
		LoopingSoundComponent->Stop();
	}
	
	Destroy();
}

//~ AActor Begin

#if WITH_EDITOR
EDataValidationResult ACL_ProjectileBase::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	// Validate ImpactEffects array
	if (ImpactEffects.Num() == 0)
	{
		Context.AddError(FText::FromString(TEXT("ImpactEffects array is empty. Please assign at least one Niagara effect.")));
		Result = EDataValidationResult::Invalid;
	}
	else
	{
		for (int32 i = 0; i < ImpactEffects.Num(); i++)
		{
			if (ImpactEffects[i] == nullptr)
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("ImpactEffects[%d] is null. Please assign a valid Niagara effect."), i)));
				Result = EDataValidationResult::Invalid;
			}
		}
	}

	// Validate ImpactSound
	if (!ImpactSound)
	{
		Context.AddError(FText::FromString(TEXT("ImpactSound is not assigned. Please assign a sound asset.")));
		Result = EDataValidationResult::Invalid;
	}

	// Validate LoopingSound
	if (!LoopingSound)
	{
		Context.AddError(FText::FromString(TEXT("LoopingSound is not assigned. Please assign a sound asset.")));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}
#endif

void ACL_ProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCapsuleBeginOverlap);

	if (LoopingSound)
	{
		LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
	}
}

//~ AActor End
