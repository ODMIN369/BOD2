// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "CPP_LevelBase.generated.h"

class ULevelSequence;

/**
 * 
 */
UCLASS()
class O369_HORROR01_API ACPP_LevelBase : public ALevelScriptActor
{
	GENERATED_BODY()
	
private:
	virtual void BeginPlay() override;

};
