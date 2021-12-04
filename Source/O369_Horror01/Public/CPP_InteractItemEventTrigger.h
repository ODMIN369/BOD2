// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_TraceInteractiveTriggerBase.h"
#include "CPP_InteractItemEventTrigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFailedTraceDelegateEvent);

/**
 * 
 */
UCLASS(BlueprintType)
class O369_HORROR01_API ACPP_InteractItemEventTrigger : public ACPP_TraceInteractiveTriggerBase
{
	GENERATED_BODY()

public:
	ACPP_InteractItemEventTrigger();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USoundBase* AcquisitionSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USoundBase* FailedSound;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<FString> RequiredItemNames;

protected:
	UFUNCTION(BlueprintCallable)
		bool UsePlayerInventoryItem();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnFailedTraceEvent();

	UPROPERTY(BlueprintAssignable)
	FFailedTraceDelegateEvent OnFailedTraceDelegateEvent;

public:
	virtual void OnTraceEvent_Implementation(FHitResult& HitResult) override;
};
