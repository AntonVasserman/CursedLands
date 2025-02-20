// Copyright Epic Games, Inc. All Rights Reserved.

#include "CursedLandsGameMode.h"
#include "CursedLandsCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACursedLandsGameMode::ACursedLandsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
