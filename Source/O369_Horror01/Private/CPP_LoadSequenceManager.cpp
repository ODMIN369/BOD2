// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_LoadSequenceManager.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "MovieSceneSequencePlayer.h"
#include "Kismet/GameplayStatics.h"

void UCPP_LoadSequenceManager::Init()
{

}

bool UCPP_LoadSequenceManager::LevelTransition(const FName NextLevelName,ULevelSequence* InScreenFadeSequence)
{
	if (NextLevelName.IsNone()) { return false; }

	ALevelSequenceActor* DummySeqActor;
	ULevelSequencePlayer* FadeSeqPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(this, InScreenFadeSequence, FMovieSceneSequencePlaybackSettings(), DummySeqActor);
	
	//FadeSeqPlayer->OnFinished.AddUnique(this, &UCPP_LoadSequenceManager::OpenLevel);
	
	return true;
}

void UCPP_LoadSequenceManager::OpenLevel(const FName InLevelName)
{
	UGameplayStatics::OpenLevel(this, InLevelName);
}