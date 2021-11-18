// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_EventTriggerBase.h"
#include "GameFramework/Actor.h"
#include "CPP_TraceEventTriggerInterface.h"
#include "CPP_TraceEventTriggerBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTraceDelegateEvent);

UCLASS()
class O369_HORROR01_API ACPP_TraceEventTriggerBase : public ACPP_EventTriggerBase ,public ICPP_TraceEventTriggerInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_TraceEventTriggerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Interface")
	void OnTraceImplementableEvent(FHitResult HitResult);

	void OnTraceEvent_Implementation(FHitResult& HitResult) override;

public:
	UPROPERTY(BlueprintAssignable)
		FTraceDelegateEvent OnTraceDelegateEvent;
};
