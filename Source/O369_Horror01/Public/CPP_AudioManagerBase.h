// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Sound/SoundClass.h"
#include "CPP_AudioManagerBase.generated.h"

class USoundBase;
class USoundAttenuation;
class USoundConcurrency;
class AActor;


/**
 * 
 */
UCLASS(BlueprintType)
class O369_HORROR01_API UCPP_AudioManagerBase : public UObject
{
	GENERATED_BODY()

public:
	UCPP_AudioManagerBase();

	void Init();

private:
	UPROPERTY()
	TSoftObjectPtr<USoundClass> MasterSoundClass;

	UPROPERTY()
	TSoftObjectPtr<USoundClass> BgmSoundClass;

	UPROPERTY()
	TSoftObjectPtr<USoundClass> SeSoundClass;

	UPROPERTY()
	TSoftObjectPtr<USoundClass> SfxSoundClass;

	UPROPERTY()
	TSoftObjectPtr<USoundClass> VoiceSoundClass;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MasterVolume = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float BgmVolume = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SeVolume = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SfxVolume = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float VoiceVolume = 1.f;

public:
	UFUNCTION(BlueprintPure)
		USoundClass* GetMasterSoundClass() { return MasterSoundClass.Get(); }

	UFUNCTION(BlueprintPure)
		USoundClass* GetBgmSoundClass() { return BgmSoundClass.Get(); }

	UFUNCTION(BlueprintPure)
		USoundClass* GetSeSoundClass() { return SeSoundClass.Get(); }

	UFUNCTION(BlueprintPure)
		USoundClass* GetSfxSoundClass() { return SfxSoundClass.Get(); }

	UFUNCTION(BlueprintPure)
		USoundClass* GetVoiceSoundClass() { return VoiceSoundClass.Get(); }

public:

	float GetBgmVolume() { return BgmVolume; }
	float GetSeVolume() { return SeVolume; }
	float GetSfxVolume() { return SfxVolume; }
	float GetVoiceVolume() { return VoiceVolume; }

	FORCEINLINE void SetMasterVolume(float InChangedVolume)
	{
		MasterSoundClass.Get()->Properties.Volume = InChangedVolume;
		MasterVolume = InChangedVolume;
	}

	FORCEINLINE void SetBgmVolume(float InChangedVolume) 
	{
		BgmSoundClass.Get()->Properties.Volume = InChangedVolume;
		BgmVolume = InChangedVolume; 
	}

	FORCEINLINE void SetSeVolume(float InChangedVolume) 
	{
		SeSoundClass.Get()->Properties.Volume = InChangedVolume;
		SeVolume = InChangedVolume; 
	}
	FORCEINLINE void SetSfxVolume(float InChangedVolume) 
	{
		SfxSoundClass.Get()->Properties.Volume = InChangedVolume;
		SfxVolume = InChangedVolume; 
	}
	FORCEINLINE void SetVoice3Volume(float InChangedVolume) 
	{
		VoiceSoundClass.Get()->Properties.Volume = InChangedVolume;
		VoiceVolume = InChangedVolume; 
	}

private:
	void PlaySound3dOr2d(const UObject* WorldContextObject, USoundBase* InSound,USoundClass* InSoundClass,float& OutDuration, float InVolume = 1.f, bool InIs3DSound = false, FVector InLocation = FVector::ZeroVector , float InPitch = 1.f, float InStartTime = 0.f, USoundAttenuation* InAttenuationSettings = nullptr, USoundConcurrency* InConcurrencySettings = nullptr, AActor* InOwningActor = nullptr);

public:
	UFUNCTION(BlueprintCallable, Category = "AudioManager", meta = (WorldContext = "WorldContextObject"))
		void PlaySoundBgm(const UObject* WorldContextObject, USoundBase* InSound, float& OutDuration, float InVolume = 1.f, bool InIs3DSound = false, FVector InLocation = FVector::ZeroVector, float InPitch = 1.f, float InStartTime = 0.f, USoundAttenuation* InAttenuationSettings = nullptr, USoundConcurrency* InConcurrencySettings = nullptr, AActor* InOwningActor = nullptr);
		void PlaySoundBgm(const UObject* WorldContextObject, USoundBase* InSound, float InVolume = 1.f, bool InIs3DSound = false, FVector InLocation = FVector::ZeroVector, float InPitch = 1.f, float InStartTime = 0.f, USoundAttenuation* InAttenuationSettings = nullptr, USoundConcurrency* InConcurrencySettings = nullptr, AActor* InOwningActor = nullptr);

	UFUNCTION(BlueprintCallable, Category = "AudioManager", meta = (WorldContext = "WorldContextObject"))
		void PlaySoundSe(const UObject* WorldContextObject, USoundBase* InSound, float& OutDuration, float InVolume = 1.f, bool InIs3DSound = false, bool InIsSendReportNoiseEvent=false, FVector InLocation = FVector::ZeroVector, float InPitch = 1.f, float InStartTime = 0.f,float InNoiseMaxRange=0.f, USoundAttenuation* InAttenuationSettings = nullptr, USoundConcurrency* InConcurrencySettings = nullptr, AActor* InOwningActor = nullptr);
		void PlaySoundSe(const UObject* WorldContextObject, USoundBase* InSound, float InVolume = 1.f, bool InIs3DSound = false,bool InIsSendReportNoiseEvent=false, FVector InLocation = FVector::ZeroVector, float InPitch = 1.f, float InStartTime = 0.f, float InNoiseMaxRange=0.f, USoundAttenuation* InAttenuationSettings = nullptr, USoundConcurrency* InConcurrencySettings = nullptr, AActor* InOwningActor = nullptr);

	UFUNCTION(BlueprintCallable, Category = "AudioManager", meta = (WorldContext = "WorldContextObject"))
		void PlaySoundSfx(const UObject* WorldContextObject, USoundBase* InSound, float& OutDuration, float InVolume = 1.f, bool InIs3DSound = false, FVector InLocation = FVector::ZeroVector, float InPitch = 1.f, float InStartTime = 0.f, USoundAttenuation* InAttenuationSettings = nullptr, USoundConcurrency* InConcurrencySettings = nullptr, AActor* InOwningActor = nullptr);
		void PlaySoundSfx(const UObject* WorldContextObject, USoundBase* InSound, float InVolume = 1.f, bool InIs3DSound = false, FVector InLocation = FVector::ZeroVector, float InPitch = 1.f, float InStartTime = 0.f, USoundAttenuation* InAttenuationSettings = nullptr, USoundConcurrency* InConcurrencySettings = nullptr, AActor* InOwningActor = nullptr);

	UFUNCTION(BlueprintCallable, Category = "AudioManager", meta = (WorldContext = "WorldContextObject"))
		void PlaySoundVoice(const UObject* WorldContextObject, USoundBase* InSound, float& OutDuration, float InVolume = 1.f, bool InIs3DSound = false, FVector InLocation = FVector::ZeroVector, float InPitch = 1.f, float InStartTime = 0.f, USoundAttenuation* InAttenuationSettings = nullptr, USoundConcurrency* InConcurrencySettings = nullptr, AActor* InOwningActor = nullptr);
		void PlaySoundVoice(const UObject* WorldContextObject, USoundBase* InSound, float InVolume = 1.f, bool InIs3DSound = false, FVector InLocation = FVector::ZeroVector, float InPitch = 1.f, float InStartTime = 0.f, USoundAttenuation* InAttenuationSettings = nullptr, USoundConcurrency* InConcurrencySettings = nullptr, AActor* InOwningActor = nullptr);
};
