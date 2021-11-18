// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPP_ParkCharaControllerInterface.generated.h"

struct FKey;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCPP_ParkCharaControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class O369_HORROR01_API ICPP_ParkCharaControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
		void MoveForward(float InAxisValue);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
		void MoveRight(float InAxisValue);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
		void Turn(float InAxisValue);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
		void LookUp(float InAxisValue);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
		void PressedSprint(FKey Key);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
		void ReleasedSprint(FKey Key);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
		void PressedPostureChange(FKey Key);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
		void ReleasedPostureChange(FKey Key);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
		void PressedJump(FKey Key);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
		void ReleasedJump(FKey Key);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
		void InteractiveAction(FKey Key);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
		void PressedInteractiveCamera(FKey Key);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
		void ReleasedInteractiveCamera(FKey Key);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
		void PressedLeanRight(FKey Key);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
		void ReleasedLeanRight(FKey Key);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
		void PressedLeanLeft(FKey Key);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ParkourCharaInterface")
		void ReleasedLeanLeft(FKey Key);
};
