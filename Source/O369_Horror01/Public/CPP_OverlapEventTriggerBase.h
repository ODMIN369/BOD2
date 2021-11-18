// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_CollisionEventTriggerBase.h"
#include "GameFramework/Actor.h"
#include "CPP_OverlapEventTriggerBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOverlapDelegateEventold);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHitDelegateEventold);

class USceneComponent;
class UBoxComponent;

UCLASS(BlueprintType)
class O369_HORROR01_API ACPP_OverlapEventTriggerBase : public ACPP_CollisionEventTriggerBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACPP_OverlapEventTriggerBase();
};
