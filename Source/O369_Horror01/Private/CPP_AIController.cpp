// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AIController.h"

ACPP_AIController::ACPP_AIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	PerceptionComponent = AIPerception;
}

void ACPP_AIController::BeginPlay()
{
	UBlackboardComponent* dummy;
	UseBlackboard(GetBlackboardData(), dummy);
	Super::BeginPlay();
}
