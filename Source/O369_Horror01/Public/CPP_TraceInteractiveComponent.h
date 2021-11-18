// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CPP_TraceInteractiveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class O369_HORROR01_API UCPP_TraceInteractiveComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCPP_TraceInteractiveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UWorld* CacheWorld;

	UPROPERTY()
	TWeakObjectPtr<AActor> CacheTraceEventTriggerActor;

#pragma region CollisionQuery
	FCollisionQueryParams InteractiveTraceQueryParams;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float InteractiveTraceDistance = 0.f;

public:
	float GetInteractiveTraceDistance() { return InteractiveTraceDistance; }

public:
	bool UpdateInteractiveTrace(FVector StartLocation, FVector EndLocation, FHitResult& HitResult,FColor DebugTraceLineColor = FColor::Red);

	bool UpdateProgressInteractiveTrace(FVector StartLocation, FVector EndLocation, FHitResult& HitResult, FColor DebugTraceLineColor = FColor::Red);

	void ReleaseInteractiveTrace();
};
