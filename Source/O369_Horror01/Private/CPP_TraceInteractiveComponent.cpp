// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_TraceInteractiveComponent.h"
#include "CPP_TraceEventTriggerInterface.h"
#include "CPP_TraceProgressEventTrigger.h"
#include "CPP_TraceInteractiveTriggerBase.h"
#include "Kismet/KismetSystemLibrary.h"

#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UCPP_TraceInteractiveComponent::UCPP_TraceInteractiveComponent()
{
	CacheWorld = GetWorld();

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UCPP_TraceInteractiveComponent::BeginPlay()
{
	Super::BeginPlay();

	InteractiveTraceQueryParams.AddIgnoredActor(GetOwner());
	InteractiveTraceQueryParams.bReturnPhysicalMaterial = true;
}

void UCPP_TraceInteractiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime,TickType,ThisTickFunction);
}

// Called every frame
bool UCPP_TraceInteractiveComponent::UpdateInteractiveTrace(FVector StartLocation, FVector EndLocation, FHitResult& HitResult, FColor DebugTraceLineColor)
{
	TArray<AActor*> DummyActors;
	if (!UKismetSystemLibrary::LineTraceSingle(CacheWorld, StartLocation, EndLocation, ETraceTypeQuery::TraceTypeQuery1, true, DummyActors, EDrawDebugTrace::Type::ForDuration, HitResult, true))
	{
		return false;
	}

	// InteractActorだった場合はInterfaceへアクセスする
	const ACPP_TraceInteractiveTriggerBase* TraceInteractiveTrigger = Cast<ACPP_TraceInteractiveTriggerBase>(HitResult.GetActor());
	if (TraceInteractiveTrigger == nullptr) { return false; }

	// ゲージ進行型のイベントトリガーの場合は無視する
	const ACPP_TraceProgressEventTrigger* ProgressEventTrigger = Cast<ACPP_TraceProgressEventTrigger>(HitResult.GetActor());
	if (ProgressEventTrigger != nullptr) { return false; }

	ICPP_TraceEventTriggerInterface* const TraceEventTrigger = Cast<ICPP_TraceEventTriggerInterface>(HitResult.GetActor());
	if (TraceEventTrigger == nullptr) { return false; }

	TraceEventTrigger->OnTraceEvent_Implementation(HitResult);

	return true;
}

bool UCPP_TraceInteractiveComponent::UpdateProgressInteractiveTrace(FVector StartLocation, FVector EndLocation, FHitResult& HitResult, FColor DebugTraceLineColor)
{
	TArray<AActor*> DummyActors;
	if(!UKismetSystemLibrary::LineTraceSingle(CacheWorld, StartLocation, EndLocation, ETraceTypeQuery::TraceTypeQuery1, true, DummyActors, EDrawDebugTrace::Type::ForDuration, HitResult, true))
	{
		return false;
	}

	// ゲージ進行型のイベントトリガーではない場合は無視する
	const ACPP_TraceProgressEventTrigger* ProgressEventTrigger = Cast<ACPP_TraceProgressEventTrigger>(HitResult.GetActor());
	if (ProgressEventTrigger == nullptr) { return false; }

	ICPP_TraceEventTriggerInterface* TraceEventTrigger = Cast<ICPP_TraceEventTriggerInterface>(HitResult.GetActor());
	if (TraceEventTrigger == nullptr) { return false; }

	if (CacheTraceEventTriggerActor == nullptr)
	{
		CacheTraceEventTriggerActor = HitResult.GetActor();
	}

	TraceEventTrigger->OnTraceEvent_Implementation(HitResult);

	return true;
}

void UCPP_TraceInteractiveComponent::ReleaseInteractiveTrace()
{
	if (CacheTraceEventTriggerActor == nullptr) { return; }

	ICPP_TraceEventTriggerInterface* TraceEventTrigger = Cast<ICPP_TraceEventTriggerInterface>(CacheTraceEventTriggerActor);

	if (TraceEventTrigger == nullptr) { return; }

	TraceEventTrigger->OnReleaseEvent_Implementation();
	CacheTraceEventTriggerActor = nullptr;
}

