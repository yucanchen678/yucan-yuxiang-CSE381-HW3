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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float CustomMaxRPM = 7000.0f;

	UFUNCTION(BlueprintCallable)
	void UpdateMaxRPM(float newMax);

	void DoSpeedUp();

	bool isSpeedDown = false;

	void EndSpeedDown();

	float SpeedDownTime = 5.0f;

	FTimerHandle SpeedDownTimerHandle;
};
