// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AmbientSound.h"
#include "CPP_O369GameInstance.h"
#include "Components/AudioComponent.h"

#pragma optimize ("",off)

void ACPP_AmbientSound::BeginPlay()
{
	Super::BeginPlay();

	UAudioComponent* AudioComp = GetAudioComponent();

	if (AudioComp == nullptr) { return; }
	if (AudioComp->Sound == nullptr) { return; }

	USoundClass* const SfxClass = UCPP_O369GameInstance::GetInstance()->GetAudioManager()->GetSfxSoundClass();
	if (SfxClass != nullptr)
	{
		AudioComp->Sound->SoundClassObject = SfxClass;
	}

	if (!bIsAutoPlay)
	{
		Super::Stop();
		return;
	}

	Super::Play();
}

#if WITH_EDITOR
// マップチェックエラーを出されるとやかましいので無効化
void ACPP_AmbientSound::CheckForErrors()
{

}
#endif

#pragma optimize ("",on)