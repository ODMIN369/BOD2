// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AICharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_ParkourCharacterBase.h"


#pragma optimize ("",off)

ACPP_AICharacterBase::ACPP_AICharacterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCPP_CharacterMovementComponent>(ACPP_CharacterBase::CharacterMovementComponentName))
{
	AITouchTraceUpdateTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AITouchTraceUpdateTimeline"));
	AITouchTraceUpdateTimeline->SetLooping(true);
}

void ACPP_AICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	FOnTimelineEvent UpdateEvent;
	UpdateEvent.BindUFunction(this,FName("AITouchTraceTimelineUpdateEvent"));
	AITouchTraceUpdateTimeline->SetTimelinePostUpdateFunc(UpdateEvent);
}

void ACPP_AICharacterBase::AITouchTraceTimelineUpdateEvent()
{
	AActor* HitResultActor;
	if (UpdateAITouchTrace(HitResultActor))
	{
		if (HitResultActor == nullptr) { return; }

		if (LastAITouchedActor.IsValid())
		{
			if (LastAITouchedActor.Get() == HitResultActor) { return; }
		}

		constexpr int32 LOCAL_PLAYER = 0;
		if (HitResultActor != UGameplayStatics::GetPlayerPawn(this, LOCAL_PLAYER)) { return; }

		LastAITouchedActor = HitResultActor;

		BeginAITouch.Broadcast(HitResultActor);
	}
	else
	{
		if (LastAITouchedActor.IsValid())
		{
			EndAITouch.Broadcast(HitResultActor);
 			LastAITouchedActor.Reset();
		}
	}
}

void ACPP_AICharacterBase::PlayAITouchTraceUpdateTimeline()
{
	if (AITouchTraceUpdateTimeline->IsPlaying()) { return; }

	AITouchTraceUpdateTimeline->Play();
}

void ACPP_AICharacterBase::StopAITouchTraceUpdateTimeline()
{
	if (!AITouchTraceUpdateTimeline->IsPlaying()) { return; }

	AITouchTraceUpdateTimeline->Stop();
}

bool ACPP_AICharacterBase::UpdateAITouchTrace(AActor*& HitResultActor)
{
	//TODO Radius,Heightはエディタ上で変更できるようにする。
	const TArray<AActor*> IgnoreActors;
	FHitResult HitResult;
	if (UKismetSystemLibrary::CapsuleTraceSingle(this, GetActorLocation(), GetActorLocation(),
		100.f, 200.f,
		AITouchTraceQuery,
		false,
		IgnoreActors,
		AITouchTraceDrawDebugType,
		HitResult,
		true))
	{
		HitResultActor = HitResult.GetActor();
		return true;
	}

	return false;
}

#pragma optimize ("",on)