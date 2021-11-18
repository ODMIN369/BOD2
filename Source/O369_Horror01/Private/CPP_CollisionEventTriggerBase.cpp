// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_CollisionEventTriggerBase.h"

#include "GameFramework/Character.h"

#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ACPP_CollisionEventTriggerBase::ACPP_CollisionEventTriggerBase()
{
	TriggerCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	TriggerCollision->SetGenerateOverlapEvents(true);

}

void ACPP_CollisionEventTriggerBase::BeginPlay()
{
	Super::BeginPlay();

	TriggerCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_CollisionEventTriggerBase::OnOverlapBegin);
	TriggerCollision->OnComponentEndOverlap.AddDynamic(this, &ACPP_CollisionEventTriggerBase::OnOverlapEnd);
	TriggerCollision->OnComponentHit.AddDynamic(this, &ACPP_CollisionEventTriggerBase::OnHit);
}

void ACPP_CollisionEventTriggerBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnOverlapBeginNativeEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ACPP_CollisionEventTriggerBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	OnOverlapEndNativeEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ACPP_CollisionEventTriggerBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	OnHitNativeEvent(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void ACPP_CollisionEventTriggerBase::OnOverlapBeginNativeEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GetIsEnableEventTrigger()) { return; }

	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character == nullptr) { return; }

	if (bIsOneShotEvent)
	{
		if (!bIsHit)
		{
			OnOverlapBeginEvent(OtherActor);
			OnOverlapBeginDelegateEvent.Broadcast(OtherActor);
			return;
		}
		return;
	}

	OnOverlapBeginEvent(OtherActor);
	OnOverlapBeginDelegateEvent.Broadcast(OtherActor);
}

void ACPP_CollisionEventTriggerBase::OnOverlapEndNativeEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!GetIsEnableEventTrigger()) { return; }

	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character == nullptr) { return; }

	if (bIsOneShotEvent)
	{
		if (!bIsHit)
		{
			OnOverlapEndEvent(OtherActor);
			OnOverlapEndDelegateEvent.Broadcast(OtherActor);
			bIsHit = true;
			return;
		}
		return;
	}

	OnOverlapEndEvent(OtherActor);
	OnOverlapEndDelegateEvent.Broadcast(OtherActor);
}

void ACPP_CollisionEventTriggerBase::OnHitNativeEvent(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetIsEnableEventTrigger()) { return; }

	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character == nullptr) { return; }

	if (bIsOneShotEvent)
	{
		if (!bIsHit)
		{
			OnHitEvent(OtherActor);
			OnHitDelegateEvent.Broadcast(OtherActor);
			bIsHit = true;
			return;
		}
		return;
	}

	OnHitEvent(OtherActor);
	OnHitDelegateEvent.Broadcast(OtherActor);
}
