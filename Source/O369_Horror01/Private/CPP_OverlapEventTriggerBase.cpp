// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_OverlapEventTriggerBase.h"

// Sets default values
ACPP_OverlapEventTriggerBase::ACPP_OverlapEventTriggerBase()
{
	TriggerCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	TriggerCollision->SetGenerateOverlapEvents(true);
}
