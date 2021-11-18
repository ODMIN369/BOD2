// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_TraceEventTriggerBase.h"

#include "Components/BoxComponent.h"

// Sets default values
ACPP_TraceEventTriggerBase::ACPP_TraceEventTriggerBase()
{
	TriggerCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	TriggerCollision->SetGenerateOverlapEvents(false);
}

// Called when the game starts or when spawned
void ACPP_TraceEventTriggerBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_TraceEventTriggerBase::OnTraceEvent_Implementation(FHitResult& HitResult)
{
	if (!bIsEnableEventTrigger) { return; }

	OnTraceImplementableEvent(HitResult);
	OnTraceDelegateEvent.Broadcast();
	//GLog->Log("Test");
}
