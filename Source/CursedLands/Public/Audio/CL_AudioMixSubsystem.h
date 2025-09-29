// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CL_AudioMixSubsystem.generated.h"

class USoundControlBus;
class USoundControlBusMix;

UCLASS()
class CURSEDLANDS_API UCL_AudioMixSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBusMix> UserMix = nullptr;
	
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBus> MasterControlBus = nullptr;
	
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBus> MusicControlBus = nullptr;
	
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBus> SFXControlBus = nullptr;
	
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBus> UIControlBus = nullptr;

	//~ UWorldSubsystem Begin
public:
	virtual void PostInitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	//~ UWorldSubsystem End
};
