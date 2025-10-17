// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WolfieCartPawn.h"
#include "WolfieCartSportsCar.generated.h"

/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class AWolfieCartSportsCar : public AWolfieCartPawn
{
	GENERATED_BODY()
	
public:

	AWolfieCartSportsCar();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	float CustomMaxRPM = 7000.0f;

	UFUNCTION(BlueprintCallable)
	void UpdateMaxRPM(float newMax);
};
