// Copyright Epic Games, Inc. All Rights Reserved.

#include "WolfieCartWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UWolfieCartWheelRear::UWolfieCartWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}