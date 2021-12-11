// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Components/TimelineComponent.h"
#include "CPP_PlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDeadDelegateEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAutoRecoveryHealthFinishedDelegateEvent);


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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTimelineComponent* AutoHealthRecoveryTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool CanIsAutoHealthRecovery = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DefaultHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AutoRecoveryHealth = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StartDelayTimeAutoRecoveryHealth = 1.f;

	UPROPERTY()
	float Health;

private:
	struct FTimerHandle TimerHandleAutoRecoveryHealth;

private:
	UFUNCTION()
	void UpdateAutoHealthRecovery();

public:
	UFUNCTION(BlueprintCallable)
	void SetHealth(float InNewHealth);

	UFUNCTION(BlueprintCallable)
	void SetHealthApplyDamage(float InDamageValue);

public:
	UFUNCTION(BlueprintPure)
	float GetDefaultHealth() const { return DefaultHealth; }

	UFUNCTION(BlueprintPure)
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure)
	bool GetIsHealthMax() const { return Health >= DefaultHealth; }

	UFUNCTION(BlueprintPure)
	bool GetIsDead() const { return Health == 0; }

public:
	UPROPERTY(BlueprintAssignable)
		FPlayerDeadDelegateEvent OnPlayerDeadDelegateEvent;

	UPROPERTY(BlueprintAssignable)
		FAutoRecoveryHealthFinishedDelegateEvent AutoRecoveryHealthFinishedDelegateEvent;
};
