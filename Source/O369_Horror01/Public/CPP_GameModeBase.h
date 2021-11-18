// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPP_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class O369_HORROR01_API ACPP_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category = "GameModeBase")
	void RestartLevel();
	
};
