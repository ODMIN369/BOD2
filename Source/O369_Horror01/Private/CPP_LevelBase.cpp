// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_LevelBase.h"
//#include "LevelSequencePlayer.h"

void ACPP_LevelBase::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = false;
}
