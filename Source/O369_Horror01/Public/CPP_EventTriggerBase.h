// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_EventTriggerBase.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class O369_HORROR01_API ACPP_EventTriggerBase : public AActor
{
	GENERATED_BODY()

public:
	ACPP_EventTriggerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();

protected:
	UPROPERTY(BlueprintReadOnly)
		USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UBoxComponent* TriggerCollision;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsEnableEventTrigger = true;

public:
	UFUNCTION(BlueprintCallable)
		void SetIsEnableEventTrigger(bool IsInEnable) { bIsEnableEventTrigger = IsInEnable; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool GetIsEnableEventTrigger() { return bIsEnableEventTrigger; }

};
