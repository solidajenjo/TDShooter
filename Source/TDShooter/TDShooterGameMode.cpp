// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TDShooterGameMode.h"
#include "TDShooterPlayerController.h"
#include "TDShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATDShooterGameMode::ATDShooterGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATDShooterPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}