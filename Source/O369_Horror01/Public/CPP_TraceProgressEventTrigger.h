// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_TraceEventTriggerBase.h"
#include "CPP_TraceProgressEventTrigger.generated.h"

/**
 * 
 */
UCLASS()
class O369_HORROR01_API ACPP_TraceProgressEventTrigger : public ACPP_TraceEventTriggerBase
{
	GENERATED_BODY()

public:
	ACPP_TraceProgressEventTrigger();

private:
	float CurrentProgressValue = 0.f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetProgressValue = 0.f;

protected:
	UFUNCTION(BlueprintCallable)
		void SetTargetProgressValue(float InNewProgressValue)
	{
		if (InNewProgressValue < 0.f) { return; }
		TargetProgressValue = InNewProgressValue;
	}

	UFUNCTION(BlueprintPure)
		float GetCurrentProgressValue() { return CurrentProgressValue; }

	UFUNCTION(BlueprintCallable)
		void ResetProgressValue() { CurrentProgressValue = 0.f; }

private:
	bool UpdateCurrentProgressValue();

public:
	void OnTraceEvent_Implementation(FHitResult& HitResult) override;
	void OnReleaseEvent_Implementation() override;
};
