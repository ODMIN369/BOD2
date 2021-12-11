// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_CharacterBase.h"
#include "Components/TimelineComponent.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CPP_AICharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAITouchTraceEvent,AActor*,AITouchedActor);

/**
 * 
 */
UCLASS(BlueprintType)
class O369_HORROR01_API ACPP_AICharacterBase : public ACPP_CharacterBase
{
	GENERATED_BODY()
	
public:
	ACPP_AICharacterBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay();

private:
	UPROPERTY()
		UTimelineComponent* AITouchTraceUpdateTimeline;

private:
	TWeakObjectPtr<AActor> LastAITouchedActor;

protected:
	UPROPERTY(EditAnywhere)
		TEnumAsByte<ETraceTypeQuery> AITouchTraceQuery = ETraceTypeQuery::TraceTypeQuery2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_AICharacterBase:Debug")
		TEnumAsByte<EDrawDebugTrace::Type> AITouchTraceDrawDebugType = EDrawDebugTrace::Type::ForDuration;

protected:
	UPROPERTY(BlueprintAssignable)
	FAITouchTraceEvent BeginAITouch;

	UPROPERTY(BlueprintAssignable)
	FAITouchTraceEvent EndAITouch;

private:
	UFUNCTION()
	void AITouchTraceTimelineUpdateEvent();

	UFUNCTION()
	bool UpdateAITouchTrace(AActor*& HitResultActor);

public:
	UFUNCTION(BlueprintCallable)
		void PlayAITouchTraceUpdateTimeline();

	UFUNCTION(BlueprintCallable)
		void StopAITouchTraceUpdateTimeline();
};
