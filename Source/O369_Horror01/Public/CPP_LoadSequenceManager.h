// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_LoadSequenceManager.generated.h"

class ULevelSequence;

/**
 * 
 */
UCLASS(BlueprintType)
class O369_HORROR01_API UCPP_LoadSequenceManager : public UObject
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void OpenLevel(const FName InLevelName);

public:
	void Init();

	UFUNCTION(BlueprintCallable)
	bool LevelTransition(const FName NextLevelName, ULevelSequence* InScreenFadeSequence);
};
