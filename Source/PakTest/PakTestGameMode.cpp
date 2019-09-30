// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PakTestGameMode.h"
#include "PakTestCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MyPlayerController.h"

APakTestGameMode::APakTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		PlayerControllerClass = AMyPlayerController::StaticClass();
	}
}
