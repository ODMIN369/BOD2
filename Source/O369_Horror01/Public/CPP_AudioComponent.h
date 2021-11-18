// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "CPP_AudioComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (CPP_Audio, Common), hidecategories = (Object, ActorComponent, Physics, Rendering, Mobility, LOD), ShowCategories = Trigger, meta = (BlueprintSpawnableComponent))
class O369_HORROR01_API UCPP_AudioComponent : public UAudioComponent
{
	GENERATED_BODY()

public:
	UCPP_AudioComponent();

private:
	void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
		TSoftObjectPtr<class USoundClass> PlayTargetSoundClass;

	UPROPERTY(EditAnywhere)
		bool bIsAutoPlay = false;
};
