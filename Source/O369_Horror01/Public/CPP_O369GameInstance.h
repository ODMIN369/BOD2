// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CPP_AudioManagerBase.h"
#include "CPP_PostProcessManager.h"
#include "CPP_PlayerInventoryBase.h"
#include "CPP_LoadSequenceManager.h"
#include "CPP_O369GameInstance.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class O369_HORROR01_API UCPP_O369GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	 virtual void Init() override;
	 virtual void OnStart() override;

public:
	UFUNCTION(BlueprintPure, Category = "O369GameInstance", DisplayName = "Get O369 GameInstance")
	static UCPP_O369GameInstance* GetInstance();

protected:
	UPROPERTY()
	UCPP_AudioManagerBase* AudioManagerBase;

	UPROPERTY()
	UCPP_PostProcessManager* PostProcessManager;

	UPROPERTY()
	UCPP_PlayerInventoryBase* PlayerInventoryBase;

	UPROPERTY()
	UCPP_LoadSequenceManager* LoadSequenceManager;

public:

	UFUNCTION(BlueprintPure)
		UCPP_AudioManagerBase* GetAudioManager() const { return AudioManagerBase; }

	UFUNCTION(BlueprintPure)
		UCPP_PostProcessManager* GetPostProcessManager() const { return PostProcessManager; }
	
	UFUNCTION(BlueprintPure)
		UCPP_PlayerInventoryBase* GetPlayerInventoryBase()
	{
		if (PlayerInventoryBase == nullptr)
		{
			PlayerInventoryBase = NewObject<UCPP_PlayerInventoryBase>();
			PlayerInventoryBase->AddToRoot();
		}
		
		return PlayerInventoryBase; 
	}

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
		void SetPlayerInputControl(const UObject* WorldContextObject, bool InIsInputControl);
};
