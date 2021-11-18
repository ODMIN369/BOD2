// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_CharacterAnimBase.h"
#include "CPP_CharacterBase.h"


void UCPP_CharacterAnimBase::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	AnimCharacter = Cast<ACPP_CharacterBase>(TryGetPawnOwner());
}

void UCPP_CharacterAnimBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (AnimCharacter == nullptr) { return; }

	Speed = AnimCharacter->GetOriginVelocityLength();
	//UE_LOG(LogTemp, Log, TEXT("%f"), Speed);
	Direction = CalculateDirection(AnimCharacter->GetVelocity(), AnimCharacter->GetActorRotation());
	UpdatePosture();
}

void UCPP_CharacterAnimBase::UpdatePosture()
{
	bIsCrouch = AnimCharacter->bIsCrouched;
	bIsJump = AnimCharacter->IsFalling();
}

void UCPP_CharacterAnimBase::SetIsSprint_Implementation(bool InIsSprint)
{
	bIsSprint = InIsSprint;
}

void UCPP_CharacterAnimBase::SetIsCrouch_Implementation(bool InIsCrouch)
{
	bIsCrouch = InIsCrouch;
}

void UCPP_CharacterAnimBase::SetIsJump_Implementation(bool InIsJump)
{
	bIsJump = InIsJump;
}

void UCPP_CharacterAnimBase::SetSurfaceType_Implementation(EPhysicalSurface SurfaceType)
{
	CurrentSurfaceType = SurfaceType;
}

void UCPP_CharacterAnimBase::SetThirdPersonMode_Implementation(bool InIsThirdPersonMode)
{
	bIsThirdPersonMode = InIsThirdPersonMode;
}

void UCPP_CharacterAnimBase::SetVerticalInputAxis_Implementation(float InVerticalAxis)
{
	VerticalInputAxis = InVerticalAxis;
}



