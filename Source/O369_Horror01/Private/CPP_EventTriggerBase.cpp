// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_EventTriggerBase.h"

#include "Components/BoxComponent.h"

ACPP_EventTriggerBase::ACPP_EventTriggerBase()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetMobility(EComponentMobility::Static);
	Root->SetComponentTickEnabled(false);

	TriggerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerCollision"));
	TriggerCollision->SetComponentTickEnabled(false);
	TriggerCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TriggerCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerCollision->SetMobility(EComponentMobility::Static);

	if (Root != nullptr)
	{
		if (TriggerCollision != nullptr)
		{
			TriggerCollision->SetupAttachment(Root);
		}
	}

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ACPP_EventTriggerBase::BeginPlay()
{
	Super::BeginPlay();
}
