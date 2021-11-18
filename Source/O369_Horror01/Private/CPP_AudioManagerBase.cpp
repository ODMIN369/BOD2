// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AudioManagerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"

UCPP_AudioManagerBase::UCPP_AudioManagerBase()
{

}

void UCPP_AudioManagerBase::Init()
{
	const FString AudioPath = TEXT("/Game/O369Studio/Audio/Class/");

	MasterSoundClass = TSoftObjectPtr<USoundClass>(FSoftObjectPath(AudioPath + TEXT("SC_Master.SC_Master"))).LoadSynchronous();
	BgmSoundClass = TSoftObjectPtr<USoundClass>(FSoftObjectPath(AudioPath + TEXT("SC_Bgm.SC_Bgm"))).LoadSynchronous();
	SeSoundClass = TSoftObjectPtr<USoundClass>(FSoftObjectPath(AudioPath + TEXT("SC_Se.SC_Se"))).LoadSynchronous();
	SfxSoundClass = TSoftObjectPtr<USoundClass>(FSoftObjectPath(AudioPath + TEXT("SC_Sfx.SC_Sfx"))).LoadSynchronous();
	VoiceSoundClass = TSoftObjectPtr<USoundClass>(FSoftObjectPath(AudioPath + TEXT("SC_Voice.SC_Voice"))).LoadSynchronous();
}

void UCPP_AudioManagerBase::PlaySound3dOr2d(const UObject* WorldContextObject, USoundBase* InSound,USoundClass* InSoundClass, float& OutDuration, float InVolume, bool InIs3dSound, FVector InLocation, float InPitch, float InStartTime, USoundAttenuation * InAttenuationSettings, USoundConcurrency * InConcurrencySettings, AActor* InOwningActor)
{
	if (InSound == nullptr) { return; }

	if (InSoundClass != nullptr)
	{
		InSound->SoundClassObject = InSoundClass;
	}

	if (InIs3dSound)
	{
		UGameplayStatics::PlaySoundAtLocation(WorldContextObject, InSound, InLocation, FRotator::ZeroRotator, InVolume, InPitch, InStartTime, InAttenuationSettings, InConcurrencySettings, InOwningActor);
	}
	else
	{
		UGameplayStatics::PlaySound2D(WorldContextObject, InSound, InVolume, InPitch, InStartTime, InConcurrencySettings, InOwningActor);
	}

	OutDuration = InSound->GetDuration();
}

void UCPP_AudioManagerBase::PlaySoundBgm(const UObject* WorldContextObject, USoundBase* InSound, float& OutDuration, float InVolume, bool InIs3dSound, FVector InLocation, float InPitch, float InStartTime, USoundAttenuation* InAttenuationSettings, USoundConcurrency* InConcurrencySettings, AActor* InOwningActor)
{
	PlaySound3dOr2d(WorldContextObject, InSound, BgmSoundClass.Get() , OutDuration, InVolume, false, FVector::ZeroVector, InPitch, InStartTime, nullptr, InConcurrencySettings, InOwningActor);
}

void UCPP_AudioManagerBase::PlaySoundBgm(const UObject* WorldContextObject, USoundBase* InSound, float InVolume, bool InIs3dSound, FVector InLocation, float InPitch, float InStartTime, USoundAttenuation* InAttenuationSettings, USoundConcurrency* InConcurrencySettings, AActor* InOwningActor)
{
	float DummyDuration = 0.f;
	PlaySound3dOr2d(WorldContextObject, InSound, BgmSoundClass.Get(), DummyDuration, InVolume, false, FVector::ZeroVector, InPitch, InStartTime, nullptr, InConcurrencySettings, InOwningActor);
}

void UCPP_AudioManagerBase::PlaySoundSe(const UObject* WorldContextObject, USoundBase* InSound, float& OutDuration, float InVolume, bool InIs3dSound, bool InIsSendReportNoiseEvent, FVector InLocation, float InPitch, float InStartTime, float InNoiseMaxRange, USoundAttenuation* InAttenuationSettings, USoundConcurrency* InConcurrencySettings, AActor* InOwningActor)
{
	PlaySound3dOr2d(WorldContextObject, InSound, SeSoundClass.Get(), OutDuration, InVolume, InIs3dSound, InLocation, InPitch, InStartTime, InAttenuationSettings, InConcurrencySettings, InOwningActor);

	if (!InIsSendReportNoiseEvent) { return; }

	constexpr float NONE_RANGE = 0.f;
	if (InNoiseMaxRange <= NONE_RANGE) { return; }

	if (InOwningActor == nullptr) { return; }

	UAISense_Hearing::ReportNoiseEvent(InOwningActor, InLocation, InVolume, InOwningActor,InNoiseMaxRange);
}

void UCPP_AudioManagerBase::PlaySoundSe(const UObject* WorldContextObject, USoundBase* InSound, float InVolume, bool InIs3dSound, bool InIsSendReportNoiseEvent, FVector InLocation, float InPitch, float InStartTime,float InNoiseMaxRange, USoundAttenuation* InAttenuationSettings, USoundConcurrency* InConcurrencySettings, AActor* InOwningActor)
{
	float DummyDuration = 0.f;

	PlaySound3dOr2d(WorldContextObject, InSound, SeSoundClass.Get(), DummyDuration, InVolume, InIs3dSound, InLocation, InPitch, InStartTime, InAttenuationSettings, InConcurrencySettings, InOwningActor);

	if (!InIsSendReportNoiseEvent) { return; }

	constexpr float NONE_RANGE = 0.f;
	if (InNoiseMaxRange <= NONE_RANGE) { return; }

	if (InOwningActor == nullptr) { return; }

	UAISense_Hearing::ReportNoiseEvent(InOwningActor, InLocation, InVolume, InOwningActor,InNoiseMaxRange);
}

void UCPP_AudioManagerBase::PlaySoundSfx(const UObject* WorldContextObject, USoundBase* InSound, float& OutDuration, float InVolume, bool InIs3dSound, FVector InLocation, float InPitch, float InStartTime, USoundAttenuation* InAttenuationSettings, USoundConcurrency* InConcurrencySettings, AActor* InOwningActor)
{
	PlaySound3dOr2d(WorldContextObject, InSound, SfxSoundClass.Get(), OutDuration, InVolume, InIs3dSound, InLocation, InPitch, InStartTime, InAttenuationSettings, InConcurrencySettings, InOwningActor);
}

void UCPP_AudioManagerBase::PlaySoundSfx(const UObject* WorldContextObject, USoundBase* InSound, float InVolume, bool InIs3dSound, FVector InLocation, float InPitch, float InStartTime, USoundAttenuation* InAttenuationSettings, USoundConcurrency* InConcurrencySettings, AActor* InOwningActor)
{
	float DummyDuration = 0.f;
	PlaySound3dOr2d(WorldContextObject, InSound, SfxSoundClass.Get(), DummyDuration, InVolume, InIs3dSound, InLocation, InPitch, InStartTime, InAttenuationSettings, InConcurrencySettings, InOwningActor);
}

void UCPP_AudioManagerBase::PlaySoundVoice(const UObject* WorldContextObject, USoundBase* InSound, float& OutDuration, float InVolume, bool InIs3dSound, FVector InLocation, float InPitch, float InStartTime, USoundAttenuation* InAttenuationSettings, USoundConcurrency* InConcurrencySettings, AActor* InOwningActor)
{
	PlaySound3dOr2d(WorldContextObject, InSound, VoiceSoundClass.Get(), OutDuration, InVolume, InIs3dSound, InLocation, InPitch, InStartTime, InAttenuationSettings, InConcurrencySettings, InOwningActor);
}

void UCPP_AudioManagerBase::PlaySoundVoice(const UObject* WorldContextObject, USoundBase* InSound, float InVolume, bool InIs3dSound, FVector InLocation, float InPitch, float InStartTime, USoundAttenuation* InAttenuationSettings, USoundConcurrency* InConcurrencySettings, AActor* InOwningActor)
{
	float DummyDuration = 0.f;
	PlaySound3dOr2d(WorldContextObject, InSound, VoiceSoundClass.Get(), DummyDuration, InVolume, InIs3dSound, InLocation, InPitch, InStartTime, InAttenuationSettings, InConcurrencySettings, InOwningActor);
}
