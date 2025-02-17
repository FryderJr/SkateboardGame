// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkateboardGameGameMode.h"
#include "SkateboardGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASkateboardGameGameMode::ASkateboardGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Skateboard/BP_Skateboard"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
