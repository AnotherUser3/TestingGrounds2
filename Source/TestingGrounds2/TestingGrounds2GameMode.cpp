// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TestingGrounds2GameMode.h"
#include "TestingGrounds2HUD.h"
#include "TestingGrounds2Character.h"
#include "UObject/ConstructorHelpers.h"

ATestingGrounds2GameMode::ATestingGrounds2GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATestingGrounds2HUD::StaticClass();
}
