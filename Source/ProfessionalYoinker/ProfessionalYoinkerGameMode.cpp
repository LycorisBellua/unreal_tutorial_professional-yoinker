// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProfessionalYoinkerGameMode.h"
#include "ProfessionalYoinkerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProfessionalYoinkerGameMode::AProfessionalYoinkerGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
