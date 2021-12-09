// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPP_ParkourCharaAnimInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCPP_ParkourCharaAnimInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class O369_HORROR01_API ICPP_ParkourCharaAnimInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "ParkourCharaInterface")
	void ChangePosture(ECharacterPosture InCharacterPosture);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
	void ChangeJumpState(bool InIsJump);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
	void ChangeAction(ECharacterObstacleAction InAction);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
	void SetWallRunDirection(float InDirectionMagnitude);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
	void SetCurrentJump(int32 InJumpLife);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
	void SetCliffMoveDirection(float InCriffMoveDir);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
	void SetCreviceMoveDirection(float InCreviceMoveVelocity);
};
