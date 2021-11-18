// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AudioComponent.h"
#include "Sound/SoundClass.h"

UCPP_AudioComponent::UCPP_AudioComponent()
{
	SetComponentTickEnabled(false);
}

void UCPP_AudioComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Sound == nullptr) { return; }

	USoundClass* const PlaySoundClass = PlayTargetSoundClass.LoadSynchronous();
	if (PlaySoundClass != nullptr) 
	{
		Sound->SoundClassObject = PlaySoundClass; 
	}

	if (bIsAutoPlay)
	{
		Play();
	}
	else
	{
		Stop();
	}
}