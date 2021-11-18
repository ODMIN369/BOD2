// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_EventTriggerBase.h"
#include "GameFramework/Actor.h"
#include "CPP_CollisionEventTriggerBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOverlapDelegateEvent,AActor*,OtherActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitDelegateEvent,AActor*, OtherActor);

class USceneComponent;
class UBoxComponent;

UCLASS(BlueprintType)
class O369_HORROR01_API ACPP_CollisionEventTriggerBase : public ACPP_EventTriggerBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACPP_CollisionEventTriggerBase();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsOneShotEvent = false;

private:
	bool bIsHit = false;

private:
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// C++Only
	virtual void OnOverlapBeginNativeEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// C++Only
	virtual void OnOverlapEndNativeEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// C++Only
	virtual void OnHitNativeEvent(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	// BP用
	UFUNCTION(BlueprintImplementableEvent)
		void OnOverlapBeginEvent(AActor* OtherActor);

	// BP用
	UFUNCTION(BlueprintImplementableEvent)
		void OnOverlapEndEvent(AActor* OtherActor);

	// BP用
	UFUNCTION(BlueprintImplementableEvent)
		void OnHitEvent(AActor* OtherActor);

public:
	// 外部からバインドする用
	UPROPERTY(BlueprintAssignable)
		FOverlapDelegateEvent OnOverlapBeginDelegateEvent;

	UPROPERTY(BlueprintAssignable)
		FOverlapDelegateEvent OnOverlapEndDelegateEvent;
	UPROPERTY(BlueprintAssignable)
		FHitDelegateEvent OnHitDelegateEvent;
};

