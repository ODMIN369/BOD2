// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sound/AmbientSound.h"
#include "CPP_AmbientSound.generated.h"

/**
 * 
 */
UCLASS()
class O369_HORROR01_API ACPP_AmbientSound : public AAmbientSound
{
	GENERATED_BODY()

private:
	void BeginPlay() override;

#if WITH_EDITOR
	virtual void CheckForErrors() override;
#endif

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool bIsAutoPlay = false;
};
