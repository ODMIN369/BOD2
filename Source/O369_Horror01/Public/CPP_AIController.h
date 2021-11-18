// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "CPP_AIController.generated.h"

/**
 * 
 */

UCLASS()
class O369_HORROR01_API ACPP_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACPP_AIController();

protected:
	virtual void BeginPlay();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAIPerceptionComponent* AIPerception;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UBlackboardData* MyBlackboardData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UBehaviorTree* MyBehaviorTree;

public:

	UFUNCTION(BlueprintPure)
		UBlackboardData* const GetBlackboardData() { return MyBlackboardData; }

	UFUNCTION(BlueprintPure)
		UBehaviorTree* const GetBehaviorTree() { return MyBehaviorTree; }
};
