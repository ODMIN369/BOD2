// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerState.h"
#include "Engine/EngineTypes.h"


DEFINE_LOG_CATEGORY_STATIC(PlayerStateLog, Log, All);

#pragma optimize ("",off)

ACPP_PlayerState::ACPP_PlayerState(const FObjectInitializer& ObjectInitializer)
{
	Health = DefaultHealth;

	AutoHealthRecoveryTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AutoHealthRecoveryTimeline"));
}

void ACPP_PlayerState::BeginPlay()
{
	Super::BeginPlay();

	AutoHealthRecoveryTimeline->SetTimelineLength(1.f);
	AutoHealthRecoveryTimeline->SetLooping(true);

	FOnTimelineEvent AutoHealthRecoveryUpdateEvent;
	AutoHealthRecoveryUpdateEvent.BindUFunction(this, FName("UpdateAutoHealthRecovery"));
	AutoHealthRecoveryTimeline->SetTimelinePostUpdateFunc(AutoHealthRecoveryUpdateEvent);
}

void ACPP_PlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_PlayerState::UpdateAutoHealthRecovery()
{
	const float BeforeHealth = Health;
	const float AfterHealth = Health + (AutoRecoveryHealth * GetWorld()->GetDeltaSeconds());
	SetHealth(AfterHealth);

	// 体力が最大値
	if (GetIsHealthMax())
	{
		UE_LOG(PlayerStateLog, Log, TEXT("Health:%f->%f"), BeforeHealth, Health);
		UE_LOG(PlayerStateLog, Log, TEXT("HealthMax!"));

		TimerHandleAutoRecoveryHealth.Invalidate();
		GetWorld()->GetTimerManager().ClearTimer(TimerHandleAutoRecoveryHealth);
		AutoHealthRecoveryTimeline->Stop();

		AutoRecoveryHealthFinishedDelegateEvent.Broadcast();
		return;
	}
}

void ACPP_PlayerState::SetHealth(float InNewHealth)
{
	constexpr float HEALTH_MIN = 0.f;
	Health = FMath::Clamp(InNewHealth, HEALTH_MIN, DefaultHealth);
}

void ACPP_PlayerState::SetHealthApplyDamage(float InDamageValue)
{
	const float BeforeHealth = Health;
	const float AfterHealth = Health - InDamageValue;
	SetHealth(AfterHealth);

	UE_LOG(PlayerStateLog, Log, TEXT("Health:%f->%f"), BeforeHealth, Health);

	// 自動回復中にダメージを受けたら自動回復を停止する
	if (AutoHealthRecoveryTimeline->IsPlaying()) 
	{
		AutoHealthRecoveryTimeline->Stop(); 
	}

	// 自動回復開始のタイマーを停止する
	if (TimerHandleAutoRecoveryHealth.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandleAutoRecoveryHealth);
		TimerHandleAutoRecoveryHealth.Invalidate();
	}

	if (GetIsDead())
	{
		UE_LOG(PlayerStateLog, Log, TEXT("OnPlayerIsDead"));
		OnPlayerDeadDelegateEvent.Broadcast();
		return;
	}

	// 自動回復の機能が有効
	if (CanIsAutoHealthRecovery)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandleAutoRecoveryHealth, [this]()
			{
				UE_LOG(PlayerStateLog, Log, TEXT("%s:UpdateAutoHealthRecovery()"),*GetNameSafe(this));
				AutoHealthRecoveryTimeline->PlayFromStart();
			}, StartDelayTimeAutoRecoveryHealth, false
		);
	}
}

#pragma optimize ("",on)