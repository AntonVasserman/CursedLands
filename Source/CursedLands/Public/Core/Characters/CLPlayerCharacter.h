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
	
	//~ ACLCharacter Begin
	// ACLCharacter End
};
