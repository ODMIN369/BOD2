// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_TraceProgressEventTrigger.h"

ACPP_TraceProgressEventTrigger::ACPP_TraceProgressEventTrigger()
{
	
}

void ACPP_TraceProgressEventTrigger::OnTraceEvent_Implementation(FHitResult& HitResult)
{
	if (!GetIsEnableEventTrigger()) { return; }
	if (!UpdateCurrentProgressValue()) { return; }

	Super::OnTraceEvent_Implementation(HitResult);
}

void ACPP_TraceProgressEventTrigger::OnReleaseEvent_Implementation()
{
	ResetProgressValue();
}

bool ACPP_TraceProgressEventTrigger:: UpdateCurrentProgressValue()
{
	CurrentProgressValue = FMath::Clamp((CurrentProgressValue + GetWorld()->GetDeltaSeconds()), 0.f, TargetProgressValue);

	if (CurrentProgressValue >= TargetProgressValue)
	{
		return true;
	}
	else
	{
		return false;
	}
}