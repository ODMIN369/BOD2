// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_ParkourCharacterBase.h"

#include "CPP_TransformComponent.h"

#include "Camera/CameraComponent.h"

#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"

#include "Kismet/KismetMathLibrary.h"

#include "GameFramework/Pawn.h"

#pragma optimize ("",off)

// ����Ɋւ��鏈���͊�{�I��Blueprint���ŌĂяo���Ă��܂��B
// C++���͕K�v�ȏ����������邮�炢

// Sets default values
ACPP_ParkourCharacterBase::ACPP_ParkourCharacterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCPP_ParkourMovementComponent>(ACPP_CharacterBase::CharacterMovementComponentName))
{
	ObstacleActionComponentRoot = CreateDefaultSubobject<UCPP_TransformComponent>(TEXT("ObstacleActionComponentRoot"));
	ObstacleActionComponentRoot->Initialize((USceneComponent*)Root);

	ObstacleTraceLocation = CreateDefaultSubobject<UCPP_TransformComponent>(TEXT("ObstacleTraceLocation"));
	ObstacleTraceLocation->Initialize(ObstacleActionComponentRoot);

	ClimbableLocation = CreateDefaultSubobject<UCPP_TransformComponent>(TEXT("ClimbableLocation"));
	ClimbableLocation->Initialize(ObstacleActionComponentRoot);

	FlyClibableLocation = CreateDefaultSubobject<UCPP_TransformComponent>(TEXT("FlyClimbableLocation"));
	FlyClibableLocation->Initialize(ObstacleActionComponentRoot);
	
	VaultableLocation = CreateDefaultSubobject <UCPP_TransformComponent>(TEXT("VaultableLocation"));
	VaultableLocation->Initialize(ObstacleActionComponentRoot);

	WallRunTraceLocation = CreateDefaultSubobject<UCPP_TransformComponent>(TEXT("WallRunTraceLocation"));
	WallRunTraceLocation->Initialize(ObstacleActionComponentRoot);
	
	TraceInteractiveComponent = CreateDefaultSubobject<UCPP_TraceInteractiveComponent>(TEXT("TraceInteractiveComponent"));

	TraceProgressInteractiveComponent = CreateDefaultSubobject<UCPP_TraceInteractiveComponent>(TEXT("TraceProgressInteractiveComponent"));

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_ParkourCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	ParkourCharacterMovement = Cast<UCPP_ParkourMovementComponent>(GetCharacterMovement());

	ParkourAnimInstance = Cast<UCPP_ParkourCharacterAnimBase>(GetMesh()->GetAnimInstance());
}

void ACPP_ParkourCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMaxMoveSpeed();
	ParkourAnimInstance->ChangePosture_Implementation(ParkourCharacterMovement->CurrentPosture);
	ParkourAnimInstance->ChangeJumpState_Implementation(ParkourCharacterMovement->IsFloating());
	UpdatePosture();
	LimitHorizontalMove();

	UpdateIsSprint();
}

void ACPP_ParkourCharacterBase::Jump()
{
	Super::Jump();
}

void ACPP_ParkourCharacterBase::VelocityJump()
{
	ParkourCharacterMovement->VelocityJump();
}

void ACPP_ParkourCharacterBase::WallJump()
{
	ParkourCharacterMovement->WallJump();
}

void ACPP_ParkourCharacterBase::ResetJump()
{
	ParkourCharacterMovement->ResetJump(ParkourCharacterMovement->MaxJumpLife);
}

void ACPP_ParkourCharacterBase::UpdateIsSprint()
{
	CharacterAnimInstance->SetIsSprint_Implementation(ParkourCharacterMovement->IsWithinTheSprintThreshold());
}

void ACPP_ParkourCharacterBase::UpdatePosture()
{
	ParkourCharacterMovement->UpdatePosture();
}

void ACPP_ParkourCharacterBase::ChangePostureStand()
{
	ParkourCharacterMovement->ChangePostureStand();
}

void ACPP_ParkourCharacterBase::BeginPosture()
{
	ParkourCharacterMovement->BeginPosture();
}

void ACPP_ParkourCharacterBase::UpdatePostureCrouch()
{
	ParkourCharacterMovement->UpdatePostureCrouch();
}

void ACPP_ParkourCharacterBase::BeginWallRun()
{
	ParkourCharacterMovement->BeginWallRun();

	ParkourAnimInstance->ChangeAction_Implementation(ParkourCharacterMovement->CurrentObstacleAction);

	//GetMesh()->SetVisibility(false);

	//�J�����X���J�n
}

void ACPP_ParkourCharacterBase::UpdateWallRun(bool& OutIsStop)
{
	ParkourCharacterMovement->UpdateWallRun(OutIsStop);

  	ParkourAnimInstance->SetWallRunDirection_Implementation(ParkourCharacterMovement->CurrentWallRunDirectionMagnitude);
}

void ACPP_ParkourCharacterBase::BeginReleaseWallRun()
{
	ParkourCharacterMovement->BeginReleaseWallRun();
}

void ACPP_ParkourCharacterBase::UpdateReleaseWallRun(bool& OutIsStop)
{
	ParkourCharacterMovement->UpdateReleaseWallRun(OutIsStop);
}

void ACPP_ParkourCharacterBase::EndWallRun(ECharacterWallRunRelaseType InWallRunReleaseType)
{
	ParkourCharacterMovement->EndWallRun(InWallRunReleaseType);

	ParkourAnimInstance->ChangeAction_Implementation(ParkourCharacterMovement->CurrentObstacleAction);
	ParkourAnimInstance->SetCurrentJump_Implementation(ParkourCharacterMovement->CurrentJumpLife);
	ParkourAnimInstance->SetWallRunDirection_Implementation(ParkourCharacterMovement->CurrentWallRunDirectionMagnitude);

	//GetMesh()->SetVisibility(true);

	//�J�����X���I��
}

float ACPP_ParkourCharacterBase::PlayObstacleActionAnimMontage(FCharacterAnimationMontageData InAnimMontageData)
{	
	return PlayAnimationMontage(InAnimMontageData) / InAnimMontageData.AnimPlayRate;
}

float ACPP_ParkourCharacterBase::PlayAnimationMontage(FCharacterAnimationMontageData InAnimMontageData)
{
	constexpr float PLAY_FAILED = 0.f;
	if (!InAnimMontageData.IsAnimMontageValid()) { return PLAY_FAILED; }

	return PlayAnimMontage(InAnimMontageData.AnimMontage, InAnimMontageData.AnimPlayRate);
}

float ACPP_ParkourCharacterBase::PlayLandedRigidityAnimMontage(float InPlayRate)
{
	if (LandedRigidityAnimMontage == nullptr) { return 0.f; }

	return PlayAnimMontage(LandedRigidityAnimMontage, InPlayRate);
}

float ACPP_ParkourCharacterBase::PlayLaddingEndAnimMontage(float InPlayRate)
{
	if (LaddingEndAnimMontage == nullptr) { return 0.f; }

	ParkourCharacterMovement->Velocity = FVector::ZeroVector;

	return PlayAnimMontage(LaddingEndAnimMontage, InPlayRate);
}

bool ACPP_ParkourCharacterBase::MovementForwardInput(float InInputScale, bool InForce = false)
{

	CurrentForwardInputAxis = InInputScale;

	ParkourCharacterMovement->SetForwardInput(InInputScale);

	constexpr float NONE_INPUT = 0.f;
	if (InInputScale == NONE_INPUT) { return false; }

	if (ParkourCharacterMovement->IsObstacleActioning())
	{
		// �悶�o��A�N�V�������͓��͂𖳌�
		if (ParkourCharacterMovement->IsObstacleClimb()) { return false; }

		// �R�ۈړ����͓��͂𖳌���
		if (ParkourCharacterMovement->IsObstacleCliff()) { return false; }

		// �n�V�S�ړ����͓��͂𖳌���
		if (ParkourCharacterMovement->IsObstacleLadder()) { return false; }

		// �j�n��A�N�V�������͓��͂𖳌�
		if (ParkourCharacterMovement->IsObstacleBalanceBeam()) { return false; }

		// ���Ԉړ��A�N�V�������͓��͂𖳌�
		if (ParkourCharacterMovement->IsObstacleCrevice()) { return false; }
	}

	ParkourAnimInstance->SetVerticalInputAxis_Implementation(InInputScale);

	if (ParkourCharacterMovement->IsCurrentPostureSliding()) { return false; }

	AddMovementInput(GetDirection(EAxis::X), InInputScale, InForce);

	return true;
}

bool ACPP_ParkourCharacterBase::MovementRightInput(float InInputScale, bool InForce = false)
{
	CurrentRightInputAxis = InInputScale;

	ParkourCharacterMovement->SetRightInput(InInputScale);

	constexpr float NONE_INPUT = 0.f;
	if (InInputScale == NONE_INPUT) { return false; }

	if (ParkourCharacterMovement->IsObstacleActioning())
	{
		// �悶�o��A�N�V�������͓��͂𖳌�
		if (ParkourCharacterMovement->IsObstacleClimb()) { return false; }

		// �n�V�S�ړ����͓��͂𖳌���
		if (ParkourCharacterMovement->IsObstacleLadder()) { return false; }

		// ��щz���A�N�V�����͔�щz�����ł����͂𖳌�
		if (ParkourCharacterMovement->IsObstacleVault()) { return false; }

		// �������A�N�V�������͓��͂𖳌�
		if (ParkourCharacterMovement->IsObstaclePush()) { return false; }

		// �j�n��A�N�V�������͓��͂𖳌�
		if (ParkourCharacterMovement->IsObstacleBalanceBeam()) { return false; }
	}

	if (ParkourCharacterMovement->IsCurrentPostureSliding())
	{
		constexpr float SLIDING_INPUT_SCALE = 0.5f;
		AddMovementInput(GetDirection(EAxis::Y), InInputScale * SLIDING_INPUT_SCALE, InForce);
	}

	AddMovementInput(GetDirection(EAxis::Y), InInputScale, InForce);

	return true;
}

void ACPP_ParkourCharacterBase::UpdateMaxMoveSpeed()
{
	ParkourCharacterMovement->UpdateMaxMoveSpeed();
}

void ACPP_ParkourCharacterBase::UpdateTraceInteractive(class UCameraComponent* Camera)
{
	// �Ȃɂ��̏�Q���A�N�V�������s���Ă���ۂ̓C���^���N�g�C�x���g�����s�ł��Ȃ�
	if (ParkourCharacterMovement->IsObstacleActioning()) { return; }

	const FVector StartTraceLocation = Camera->GetComponentLocation();
	const FVector EndTraceLocation = StartTraceLocation + (Camera->GetForwardVector() * TraceInteractiveComponent->GetInteractiveTraceDistance());
	FHitResult HitResult;
	TraceInteractiveComponent->UpdateInteractiveTrace(StartTraceLocation, EndTraceLocation, HitResult);
}

void ACPP_ParkourCharacterBase::UpdateProgressTraceInteractive(class UCameraComponent* Camera)
{
	// �Ȃɂ��̏�Q���A�N�V�������s���Ă���ۂ̓C���^���N�g�C�x���g�����s�ł��Ȃ�
	if (ParkourCharacterMovement->IsObstacleActioning()) { return; }

	const FVector StartTraceLocation = Camera->GetComponentLocation();
	const FVector EndTraceLocation = StartTraceLocation + (Camera->GetForwardVector() * TraceInteractiveComponent->GetInteractiveTraceDistance());
	FHitResult HitResult;
	if (!TraceProgressInteractiveComponent->UpdateProgressInteractiveTrace(StartTraceLocation, EndTraceLocation, HitResult, FColor::Blue))
	{
		TraceProgressInteractiveComponent->ReleaseInteractiveTrace();
	}
}

bool ACPP_ParkourCharacterBase::CalculateObstacleAction(ECharacterObstacleAction& OutObstacleAction, FVector& OutActioningLocation)
{
	return ParkourCharacterMovement->CalculateObstacleAction(OutObstacleAction, OutActioningLocation);
}

void ACPP_ParkourCharacterBase::LimitHorizontalMove()
{
	ParkourCharacterMovement->LimitHorizontalMove();
}

void ACPP_ParkourCharacterBase::ChangeWallRunParameter(ECharacterWallRunState InWallRunState)
{
	ParkourCharacterMovement->ChangeWallRunParameter(InWallRunState);
}

void ACPP_ParkourCharacterBase::FindWallRunDirectionAndSide(FVector InHitLocation, ECharacterWallRunSide& OutWallRunSide, FVector& OutDirection)
{
	ParkourCharacterMovement->FindWallRunDirectionAndSide(InHitLocation, OutWallRunSide, OutDirection);
}

#pragma optimize ("",on)



