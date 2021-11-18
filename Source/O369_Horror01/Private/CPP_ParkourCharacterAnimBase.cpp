// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ParkourCharacterAnimBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CPP_ParkourCharacterBase.h"

#pragma optimize ("",off)

void UCPP_ParkourCharacterAnimBase::NativeBeginPlay()
{
	Super::NativeBeginPlay();


	if (AnimCharacter == nullptr) { return; }
	ParkourAnimCharacter = Cast<ACPP_ParkourCharacterBase>(AnimCharacter);
}

void UCPP_ParkourCharacterAnimBase::NativeUpdateAnimation(float DeltaSeconds)
{
	UpdateObstacleAction();

	if (ParkourAnimCharacter != nullptr)
	{
		Speed = 0.f;
		if (ParkourAnimCharacter.Get()->GetParkourMovementComponent()->IsObstacleActioning()) { return; }
	}
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UCPP_ParkourCharacterAnimBase::UpdatePosture()
{
	bIsCrouch = CharacterPosture == ECharacterPosture::Crouch;
	bIsSliding = CharacterPosture == ECharacterPosture::Sliding;
}

void UCPP_ParkourCharacterAnimBase::UpdateObstacleAction()
{
	bIsVault = ObstacleAction == ECharacterObstacleAction::Vault;
	bIsClimb = ObstacleAction == ECharacterObstacleAction::Clamb;
	bIsWallRun = ObstacleAction == ECharacterObstacleAction::WallRun;
	bIsCliff = ObstacleAction == ECharacterObstacleAction::Cliff;
	bIsHanging = ObstacleAction == ECharacterObstacleAction::Hanging;
}

void UCPP_ParkourCharacterAnimBase::ChangePosture_Implementation(ECharacterPosture InCharacterPosture)
{
	CharacterPosture = InCharacterPosture;
}

void UCPP_ParkourCharacterAnimBase::ChangeJumpState_Implementation(bool InIsJump)
{
	bIsJump = InIsJump;
}

void UCPP_ParkourCharacterAnimBase::ChangeAction_Implementation(ECharacterObstacleAction InAction)
{
	ObstacleAction = InAction;
}

void UCPP_ParkourCharacterAnimBase::SetWallRunDirection_Implementation(float InDirectionMagnitude)
{
  	WallRunDirection = InDirectionMagnitude;
}

void UCPP_ParkourCharacterAnimBase::SetCurrentJump_Implementation(int32 InJumpLife)
{
	CurrentJump = InJumpLife;
}

void UCPP_ParkourCharacterAnimBase::SetCliffMoveDirection_Implementation(float InCliffMoveDir)
{
	CliffMoveDir = CliffMoveDir;
}

#pragma optimize ("",on)