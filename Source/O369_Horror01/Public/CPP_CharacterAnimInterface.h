// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Engine/EngineTypes.h"
#include "CPP_CharacterAnimInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCPP_CharacterAnimInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class O369_HORROR01_API ICPP_CharacterAnimInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterAnimInterface")
		void SetIsSprint(bool InIsSprint);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterAnimInterface")
		void SetIsCrouch(bool InIsCrouch);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterAnimInterface")
		void SetIsJump(bool InIsJump);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterAnimInterface")
		void SetSurfaceType(EPhysicalSurface SurfaceType);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterAnimInterface")
		void SetThirdPersonMode(bool InIsThirdPersonMode);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterAnimInterface")
		void SetVerticalInputAxis(float InVerticalAxis);
};
