// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ActionCharacterAnimBase.h"

void UCPP_ActionCharacterAnimBase::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UCPP_ActionCharacterAnimBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UCPP_ActionCharacterAnimBase::SetAds_Implementation(bool InIsAds)
{
	bIsAds = InIsAds;
}