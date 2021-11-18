// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CPP_PlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDeadDelegateEvent);

/**
 * 
 */
UCLASS()
class O369_HORROR01_API ACPP_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_PlayerState(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	float Health;

public:
	UFUNCTION(BlueprintCallable)
	void SetHealthApplyDamage(float InDamageValue);

	UFUNCTION(BlueprintPure)
	float GetHealth() { return Health; }

	UFUNCTION(BlueprintPure)
		bool GetIsDead() { return Health == 0; }

public:
	UPROPERTY(BlueprintAssignable)
		FPlayerDeadDelegateEvent OnPlayerDeadDelegateEvent;
};
