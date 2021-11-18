// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_O369GameInstance.h"

#include "Kismet/GameplayStatics.h"

#include "Engine/Engine.h"

UCPP_O369GameInstance* UCPP_O369GameInstance::GetInstance()
{
	UCPP_O369GameInstance* Instance = nullptr;

	if (!GEngine) { return nullptr; }

	FWorldContext* Context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
	Instance = Cast<UCPP_O369GameInstance>(Context->OwningGameInstance);

	if (Instance) { return Instance; }

	Instance = NewObject<UCPP_O369GameInstance>();

	return Instance;
}

void UCPP_O369GameInstance::Init()
{
	Super::Init();
}

void UCPP_O369GameInstance::OnStart()
{
	Super::OnStart();

	AudioManagerBase = NewObject<UCPP_AudioManagerBase>();
	AudioManagerBase->AddToRoot();
	AudioManagerBase->Init();

	PostProcessManager = NewObject<UCPP_PostProcessManager>();
	PostProcessManager->AddToRoot();

	LoadSequenceManager = NewObject<UCPP_LoadSequenceManager>();
	LoadSequenceManager->Init();
}

void UCPP_O369GameInstance::SetPlayerInputControl(const UObject* WorldContextObject, bool InIsInputControl)
{
	constexpr int32 CLIENT_LOCAL_PLAYER = 0;

	APawn* const PlayerPawn = UGameplayStatics::GetPlayerPawn(WorldContextObject, CLIENT_LOCAL_PLAYER);

	if (PlayerPawn == nullptr) { return; }

	APlayerController* const PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, CLIENT_LOCAL_PLAYER);

	if (PlayerController == nullptr) { return; }

	//InIsInputControl == true ? PlayerPawn->EnableInput(PlayerController) : PlayerPawn->DisableInput(PlayerController);

	//•K—v‚É‰ž‚¶‚Ä‚±‚¿‚ç‚ÉˆÈ‰º‚ÉØ‚è‘Ö‚¦
	PlayerController->SetCinematicMode(!InIsInputControl, true, true);
}