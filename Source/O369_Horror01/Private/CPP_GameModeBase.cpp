// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_GameModeBase.h"
#include "Kismet/GameplayStatics.h"

void ACPP_GameModeBase::RestartLevel()
{
	const FName CurrentLevelName = FName(*UGameplayStatics::GetCurrentLevelName(this));
	UGameplayStatics::OpenLevel(this, CurrentLevelName);
}
