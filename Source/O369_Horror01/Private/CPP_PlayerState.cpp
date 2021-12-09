// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(PlayerStateLog, Log, All);

#pragma optimize ("",off)

ACPP_PlayerState::ACPP_PlayerState(const FObjectInitializer& ObjectInitializer)
{
	constexpr float DEFAULT_HEALTH = 100.f;
	Health = DEFAULT_HEALTH;
}

void ACPP_PlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_PlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_PlayerState::SetHealthApplyDamage(float InDamageValue)
{
	const float BeforeHealth = Health;
	const float AfterHealth = Health - InDamageValue;
	Health = FMath::Clamp(AfterHealth, 0.f, Health);

	UE_LOG(PlayerStateLog, Log, TEXT("Health:%f->%f"), BeforeHealth, Health);

	if (GetIsDead())
	{
		UE_LOG(PlayerStateLog, Log, TEXT("OnPlayerIsDead"));
		OnPlayerDeadDelegateEvent.Broadcast();
	}
}

#pragma optimize ("",on)