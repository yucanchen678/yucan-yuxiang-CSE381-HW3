// Copyright Epic Games, Inc. All Rights Reserved.

#include "WolfieCartGameMode.h"
#include "WolfieCartPlayerController.h"

AWolfieCartGameMode::AWolfieCartGameMode()
{
	PlayerControllerClass = AWolfieCartPlayerController::StaticClass();
}
