// Copyright Epic Games, Inc. All Rights Reserved.

#include "WolfieCartWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UWolfieCartWheelFront::UWolfieCartWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}