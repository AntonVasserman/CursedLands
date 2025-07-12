// Copyright Anton Vasserman, All Rights Reserved.


#include "Animation/CL_AnimInstance.h"

#include "Characters/CL_Character.h"

//~ UAnimInstance Begin

void UCL_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (ACL_Character* OwningCharacter = Cast<ACL_Character>(GetOwningActor()))
	{
		Character = OwningCharacter;
	}
}

void UCL_AnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (Character)
	{
// We don't want to run this code in the editor because it relies on the GAS Component.
// The GAS Component is initialized only on BeginPlay so we shouldn't do this code in the Editor.
#ifndef WITH_EDITOR
		bAlive = Character->IsAlive();
#endif
	}
}

//~ UAnimInstance End
