// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ParkourMovementComponent.h"

#include "CPP_TransformComponent.h"

#include "CPP_ParkourCharacterBase.h"
#include "CPP_CharacterMovementComponent.h"

#include "CPP_TraceObstacleTriggerBase.h"
#include "CPP_TraceEventTriggerInterface.h"

#include "CPP_PlayerController.h"

#include "CPP_O369GameInstance.h"

#include "Components/CapsuleComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(ParkourMovementCompLog, Log, All);

#pragma optimize ("",off)

UCPP_ParkourMovementComponent::UCPP_ParkourMovementComponent()
{
	SprintMoveSpeed = 800.f;
	WalkMoveSpeed = 200.f;
	MaxFlySpeed = 0.f;
	BrakingDecelerationFlying = 500.f;
	JogingMoveSpeed = 200.f;
	CrouchMoveSpeed = 100.f;
	WaterMoveSpeedRate = 0.7f;
	SlidingBoostPower = 550.f;
	SlidingThreshold = 100.f;
	LadderClimbSpeed = 50.f;
	CliffMoveSpeed = 50.f;
	CreviceMoveSpeed = 50.f;

	bIsSprintForwardOnly = true;
	bIsCanWallRun = false;

	MaxJumpLife = 1;

	WallRunMoveSpeed = 1500.f;
	WallRunGravityScale = 5.f;
	WallRunReleaseDeceleration = 350.f;
	WallRunReleaseGravity = 20.f;
	WallJumpUpPower = 500.f;
	WallJumpSidePower = 500.f;

	SprintThreshold = 500.f;
	WallRunThreshold = 800.f;;
	VaultMoveAddDistance = 0.f;

	HorizontalLimitVelocity = 10000.f;

	GravityScale = 4.f;
	GroundFriction = 7.f;
	MaxWalkSpeed = 500.f;
	MaxWalkSpeedCrouched = 250.f;
	BrakingDecelerationWalking = 700.f;
	BrakingFrictionFactor = 3.f;
	bCanWalkOffLedgesWhenCrouching = true;

	JumpZVelocity = 700.f;
	AirControl = 1.f;

	NavAgentProps.bCanCrouch = true;
	NavAgentProps.bCanSwim = false;

	bUpdateOnlyIfRendered = true;
}

void UCPP_ParkourMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	ParkourCharacter = Cast<ACPP_ParkourCharacterBase>(GetCharacterOwner());

	ParkourCharacter->LandedDelegate.AddDynamic(this, &UCPP_ParkourMovementComponent::Landed);

	if (ParkourCharacter->GetIsThirdPersonMode())
	{
		bOrientRotationToMovement = true;
		bIsSprintForwardOnly = false;
	}

	DefaultGravityScale = GravityScale;

	CurrentJumpLife = MaxJumpLife;

	WaterWalkMoveSpeed = WalkMoveSpeed * WaterMoveSpeedRate;
	WaterJogingMoveSpeed = JogingMoveSpeed * WaterMoveSpeedRate;
	WaterSprintMoveSpeed = SprintMoveSpeed * WaterMoveSpeedRate;
}

void UCPP_ParkourMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateMaxMoveSpeed();
	UpdatePosture();
	LimitHorizontalMove();
}

void UCPP_ParkourMovementComponent::ObstacleLadderMovementInput(float InInputScale)
{
	MaxFlySpeed = LadderClimbSpeed;
	FVector UpVector = ParkourCharacter->GetActorUpVector();
	ParkourCharacter->AddMovementInput(UpVector * MaxFlySpeed, InInputScale);
	ParkourCharacter->GetParkourCharacterAnimInstance()->SetVerticalInputAxis_Implementation(InInputScale);
}

void UCPP_ParkourMovementComponent::ObstacleCliffMovementInput(float InInputScale)
{
	MaxFlySpeed = CliffMoveSpeed;
	FVector ForwardVector = ParkourCharacter->GetMesh()->GetForwardVector();
	ParkourCharacter->AddMovementInput(-ForwardVector * MaxFlySpeed, InInputScale);
	ParkourCharacter->GetParkourCharacterAnimInstance()->SetCliffMoveDirection_Implementation(InInputScale);
}

void UCPP_ParkourMovementComponent::ObstacleCreviceMovementInput(float InInputScale)
{
	MaxFlySpeed = CreviceMoveSpeed;
	FVector ForwardVector = ParkourCharacter->GetMesh()->GetForwardVector();
	ParkourCharacter->AddMovementInput(-ForwardVector * MaxFlySpeed, InInputScale);
	ParkourCharacter->GetParkourCharacterAnimInstance()->SetCreviceMoveDirection_Implementation(InInputScale);
}

void UCPP_ParkourMovementComponent::ObstacleBalanceBeamMovementInput(float InInputScale)
{
	MaxFlySpeed = BalanceBeamMoveSpeed;
	FVector ForwardVector = ParkourCharacter->GetActorForwardVector();
	ParkourCharacter->AddMovementInput(ForwardVector * MaxFlySpeed, InInputScale);
	//ParkourCharacter->GetParkourCharacterAnimInstance()->SetCliffMoveDirection_Implementation(InInputScale);
}

void UCPP_ParkourMovementComponent::VelocityJump()
{
 	if (!IsCanJump()) { return; }
	if (IsObstacleActioning()) { return; }
	if (IsHighWaterSurface() || IsMidWaterSurface()) { return; }

	CurrentJumpLife--;
	JumpContinuousUseCount++;

	const float VelocityLength = GetOriginVelocityLength();
	// ???????x?????l
	constexpr float WALK_VELOCITY_LENGTH_THRESHOLD = 20.f;
	// ???Q???????????W?????v?????????????????????????o?O??????
	const float VelocityRate = FMath::Clamp(VelocityLength / WALK_VELOCITY_LENGTH_THRESHOLD, 0.f, 1.f);

	const float JumpPower = JumpZVelocity - (JumpPowerDecelerateValue * JumpContinuousUseCount);
	const float ClampJumpPower = FMath::Clamp(JumpPower, MinimalJumpPower, JumpZVelocity);
	const FVector AfterVelocity = Velocity.GetSafeNormal() * (VelocityLength + (JumpingAddForwardPower * VelocityRate));
	const FVector JumpVelocity = FVector(AfterVelocity.X ,AfterVelocity.Y, ClampJumpPower);

	Launch(JumpVelocity);
	ChangePostureStand();

	ParkourCharacter->SetCurrentJump_Implementation(GetCurrentJumpLife());

	if (!IsContinuousUseSliding()) { return; }
	EndSlidingEvent();
}

void UCPP_ParkourMovementComponent::UpdateMaxMoveSpeed()
{
	if (IsObstacleWallRun()) { return; }
	if (!IsMovingOnGround()) { return; }

	// ??????????????????????????
	if (IsHighWaterSurface())
	{
		SetMoveSpeed(WaterWalkMoveSpeed);
		return;
	}

	//?@??????????
	if (IsCurrentPostureCrouching())
	{
		SetMoveSpeed(CrouchMoveSpeed);
		MaxWalkSpeedCrouched = CrouchMoveSpeed;
		return;
	}


	// ???[?J??????==========================================================================================================================
	// ?n?`?????????????x???X?V
	auto LocalUpdateSprintMove = [this]()
	{
		IsMidWaterSurface() == true ? SetMoveSpeed(WaterSprintMoveSpeed) : SetMoveSpeed(SprintMoveSpeed);
	};

	// ?n?`???????W???M???O???x???X?V
	auto LocalUpdateJogingMove = [this]()
	{
		IsMidWaterSurface() == true ? SetMoveSpeed(WaterJogingMoveSpeed) : SetMoveSpeed(JogingMoveSpeed);
	};
	// =====================================================================================================================================

	if (bIsSprintForwardOnly)
	{
		if (ParkourCharacter->IsMovingForward())
		{
			if (GetPlayerController()->GetIsSprintInput())
			{
				bIsJogingMode ? LocalUpdateJogingMove() : LocalUpdateSprintMove();
				return;
			}
		}
		SetMoveSpeed(WalkMoveSpeed);
		return;
	}
	else
	{
		if (ParkourCharacter->IsMovingForward())
		{
			if (GetPlayerController()->GetIsSprintInput())
			{
				bIsJogingMode ? LocalUpdateJogingMove() : LocalUpdateSprintMove();
				return;
			}
		}
		else
		{
			SetMoveSpeed(WalkMoveSpeed);
			return;
		}
	}
}

void UCPP_ParkourMovementComponent::WallJump()
{
	CurrentJumpLife--;

	const FVector LaunchDirection = FVector(ParkourCharacter->GetActorRightVector()) + FVector(ParkourCharacter->GetActorForwardVector());
	
	const FVector UpVector = ParkourCharacter->GetActorUpVector() * WallJumpUpPower;
	const FVector RightVector = CurrentWallRunSide == ECharacterWallRunSide::Right ? ParkourCharacter->GetActorRightVector() * WallJumpSidePower : ParkourCharacter->GetActorRightVector() * -WallJumpSidePower;
	const FVector ForwardVector = ParkourCharacter->GetActorForwardVector() * WallJumpForwardPower;
	const FVector AddJumpVelocity = (RightVector * FMath::Abs(FMath::Clamp(RightInput, 0.5f, 1.f))) + ForwardVector;

	const FVector LaunchVelocity = UpVector + AddJumpVelocity;
	ParkourCharacter->LaunchCharacter(LaunchVelocity, true, true);

	if (!IsContinuousUseSliding()) { return; }

	ResetContinuousUseSlidingCount();
}

void UCPP_ParkourMovementComponent::ResetJump(int32 InChangedJump)
{
	constexpr int32 MIN_JUMP = 0;
	CurrentJumpLife = FMath::Clamp(InChangedJump, MIN_JUMP, MaxJumpLife);
}

void UCPP_ParkourMovementComponent::Landed(const FHitResult& Hit)
{
	constexpr int32 USED_JUMP = 0;
	if (JumpContinuousUseCount > USED_JUMP)
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();

		if (TimerManager.IsTimerActive(JumpDecelerateCoolTimeHandle))
		{
			TimerManager.ClearTimer(JumpDecelerateCoolTimeHandle);
		}
		GetWorld()->GetTimerManager().SetTimer(JumpDecelerateCoolTimeHandle,
			[this]()
			{
				JumpContinuousUseCount = 0;
				UE_LOG(ParkourMovementCompLog, Log, TEXT("JumpDecelerateCoolTime : Complete JumpPower Reset!"));
			},
			JumpPowerDecelerateCoolTime, false);
	}

	UE_LOG(ParkourMovementCompLog, Log, TEXT("Process Landed()"));

	constexpr float RIGIDITY_LANDED_ENABLE_THRESHOLD = 0.f;
	// ???n?d?????D???????F??
	if (HeavyLandedRigidityThreshold > RIGIDITY_LANDED_ENABLE_THRESHOLD)
	{
		if (IsHeavyLandedRigidity())
		{
			HeavyLandedRigidity();
			OnHeavyRigidityLandedDelegateEvent.Broadcast();
			return;
		}
	}

	// ???n?d?????D???????F??
	if (SoftLandedRigidityThreshold > RIGIDITY_LANDED_ENABLE_THRESHOLD)
	{
		if (IsSoftLandedRigidity())
		{
			SoftLandedRigidity();
			OnSoftRigidityLandedDelegateEvent.Broadcast();
			return;
		}
	}

	// ???n?d?????D???????F??
	if (bIsJumpLandedRigidityEnabled)
	{
		if (IsJumpLandedRigidity())
		{
			JumpLandedRigidity();
			OnNormalLandedDelegateEvent.Broadcast();
			return;
		}
	}

	LandedSliding();

	OnNormalLandedDelegateEvent.Broadcast();
}

void UCPP_ParkourMovementComponent::HeavyLandedRigidity()
{
	UE_LOG(ParkourMovementCompLog, Log, TEXT("ZVelocity: %f,HeavyLandedRigidityThreshold: %f"), FMath::Abs(Velocity.Z), HeavyLandedRigidityThreshold);

	Velocity = FVector::ZeroVector;

	UCPP_O369GameInstance::GetInstance()->SetPlayerInputControl(this, false);
	//UE_LOG(ParkourMovementCompLog, Log, TEXT("HeavyLandedRigidityTime : Complete PlayerInputControl Disable!"));

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(HeavyLandedRigidityTimeHandle))
	{
		TimerManager.ClearTimer(HeavyLandedRigidityTimeHandle);
	}
	GetWorld()->GetTimerManager().SetTimer(HeavyLandedRigidityTimeHandle,
		[this]()
		{
			UCPP_O369GameInstance::GetInstance()->SetPlayerInputControl(this, true);
			UE_LOG(ParkourMovementCompLog, Log, TEXT("HeavyLandedRigidityTime : Complete PlayerInputControl Enabled!"));
		},
		HeavyLandedRigidityTime, false
		);

	ParkourCharacter->PlayLandedRigidityAnimMontage();

	if (HeavyLandedRigidityCameraShake == nullptr) { return; }
	GetPlayerController()->ClientStartCameraShake(HeavyLandedRigidityCameraShake);

}

void UCPP_ParkourMovementComponent::SoftLandedRigidity()
{
	UE_LOG(ParkourMovementCompLog, Log, TEXT("ZVelocity: %f,SoftLandedRigidityThreshold: %f"), FMath::Abs(Velocity.Z), HeavyLandedRigidityThreshold);

	Velocity = FVector::ZeroVector;

	UCPP_O369GameInstance::GetInstance()->SetPlayerInputControl(this, false);
	//UE_LOG(ParkourMovementCompLog, Log, TEXT("SoftLandedRigidityTime : Complete PlayerInputControl Disable!"));

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(SoftLandedRigidityTimeHandle))
	{
		TimerManager.ClearTimer(SoftLandedRigidityTimeHandle);
	}
	GetWorld()->GetTimerManager().SetTimer(SoftLandedRigidityTimeHandle,
		[this]()
		{
			UCPP_O369GameInstance::GetInstance()->SetPlayerInputControl(this, true);
			UE_LOG(ParkourMovementCompLog, Log, TEXT("SoftLandedRigidityTime : Complete PlayerInputControl Enabled!"));
		},
		SoftLandedRigidityTime, false
			);

	if (SoftLandedRigidityCameraShake == nullptr) { return; }
	GetPlayerController()->ClientStartCameraShake(SoftLandedRigidityCameraShake);
}

void UCPP_ParkourMovementComponent::JumpLandedRigidity()
{
	Velocity = FVector::ZeroVector;

	UCPP_O369GameInstance::GetInstance()->SetPlayerInputControl(this, false);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(JumpLandedRigidityTimeHandle))
	{
		TimerManager.ClearTimer(JumpLandedRigidityTimeHandle);
	}
	GetWorld()->GetTimerManager().SetTimer(JumpLandedRigidityTimeHandle,
		[this]()
		{
			UCPP_O369GameInstance::GetInstance()->SetPlayerInputControl(this, true);
			UE_LOG(ParkourMovementCompLog, Log, TEXT("JumpLandedRigidityTime : Complete PlayerInputControl Enabled!"));
		},
		JumpLandedRigidityTime, false
			);
}

void UCPP_ParkourMovementComponent::BeginSlidingEvent()
{
	SlidingContinuousUseCount++;

	ACPP_PlayerController* const PlayerController = GetPlayerController();
	if (PlayerController == nullptr) { return; }

	//?J???????c???]????
	constexpr float CAM_VERTICAL_VIEW_MAX = 45.f;
	constexpr float CAM_VERTICAL_VIEW_MIN = -20.f;
	PlayerController->SetCameraVerticalView(CAM_VERTICAL_VIEW_MAX, CAM_VERTICAL_VIEW_MIN);

	//?J???????????]????
	constexpr float CAM_HORIZON_VIEW_MAX = 90.f;
	constexpr float CAM_HORIZON_VIEW_MIN = -15.f;
	PlayerController->SetCameraHorizonView(CAM_HORIZON_VIEW_MAX, CAM_HORIZON_VIEW_MIN);

	BeginSlidingDelegateEvent.Broadcast();

	UE_LOG(ParkourMovementCompLog, Log, TEXT("BeginSliding"));
}

void UCPP_ParkourMovementComponent::Sliding()
{
	if (!bIsCanSliding) { return; }
	if (!IsWithinTheSprintThreshold()) { return; }

	const float BeforeVelocitySize = GetOriginVelocityLength();
	const float MaxBeforeVelocitySize = bIsLimitSlidingMaxPower ? SlidingMaxPower : BeforeVelocitySize;
	// (???O?????x+?X???C?f?B???O????????) - (?X???C?f?B???O?????????? * ?A???X???C?f?B???O????)
	const float SlidingPower = (FMath::Clamp(BeforeVelocitySize,0.f, MaxBeforeVelocitySize) + SlidingBoostPower) - (SlidingDecelerateValue * SlidingContinuousUseCount);
	// ?X???C?f?B???O?????????????????????????????????l???????l??????
	const float SlidingMaxPowerValue = bIsLimitSlidingMaxPower == true ? SlidingMaxPower : SlidingPower;
	const float ClampSlidingPower = FMath::Clamp(SlidingPower, SlidingMinPower, SlidingMaxPowerValue);
	const FVector VelocityNormal = Velocity.GetSafeNormal();
	//const FVector AfterVelocity = VelocityNormal * ClampSlidingPower;

	// ???K?????????????? * ?X???C?f?B???O??
	AfterVelocity = VelocityNormal * ClampSlidingPower;

	Velocity = AfterVelocity;

	//ParkourCharacter->AddMovementInput(AfterVelocity);

	UE_LOG(ParkourMovementCompLog, Log, TEXT("SlidingVelocity:%f"), SlidingPower);
	UE_LOG(ParkourMovementCompLog, Log, TEXT("LimitSlidingVelocity:%f"), MaxBeforeVelocitySize);

	// ?X???C?f?B???O???????O??Velocity(?X???C?f?B???O???\???x)?????????????x???o??????????????BeginSlidingEvent?????s??????
	if (GetOriginVelocityLength() < BeforeVelocitySize)
	{
		UE_LOG(ParkourMovementCompLog, Log, TEXT("BeforeVelocity:%f"), BeforeVelocitySize);
		UE_LOG(ParkourMovementCompLog, Log, TEXT("AfterVelocity:%f"), GetOriginVelocityLength());
		return; 
	}

	SetPosture(ECharacterPosture::Sliding);

	BrakingDecelerationWalking = BrakingDecelerationSliding;
	CrouchedHalfHeight = SlidingCollisionHalfHeight;
	ParkourCharacter->Crouch();

	if (GetHorizontalVelocityLength() >= MaxBeforeVelocitySize)
	{
		FullPowerSlidingDelegateEvent.Broadcast();
	}

	BeginSlidingEvent();
}

void UCPP_ParkourMovementComponent::LandedSliding()
{
	if (!GetPlayerController()->GetIsCrouchInput())
	{
		ChangePostureStand();
		return;
	}
	if (!IsWithinTheSlidingThreshold()) { return; }
	if (IsObstacleWallRun()) { return; }
	if (IsMidWaterSurface()) { return; }
	if (IsHighWaterSurface()) { return; }
	Sliding();
}

void UCPP_ParkourMovementComponent::EndSlidingEvent()
{
	constexpr float DEFAULT_BRAKING_DECELERATION = 5000.f;
	BrakingDecelerationWalking = DEFAULT_BRAKING_DECELERATION;

	if (IsContinuousUseSliding())
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();

		if (TimerManager.IsTimerActive(SlidingDecelerateCoolTimeHandle))
		{
			TimerManager.ClearTimer(SlidingDecelerateCoolTimeHandle);
		}
		GetWorld()->GetTimerManager().SetTimer(SlidingDecelerateCoolTimeHandle, 
			[this]() 
			{
				ResetContinuousUseSlidingCount();
				UE_LOG(ParkourMovementCompLog, Log, TEXT("SlidingDecelerateCoolTime : Complete reset!"));
			},
			SlidingDecelerateCoolTime,false);
	}

	ACPP_PlayerController* const PlayerController = GetPlayerController();
	if (PlayerController == nullptr) { return; }

	//?J???????c?????]????????
	PlayerController->ResetCameraVerticalView();

	//?J?????????????]????????
	PlayerController->ResetCameraHorizonView();

	EndSlidingDelegateEvent.Broadcast();

	UE_LOG(ParkourMovementCompLog, Log, TEXT("SlidingEnd"));
}

ECharacterPosture UCPP_ParkourMovementComponent::SetPosture(ECharacterPosture InPosture)
{
	if (InPosture != ECharacterPosture::Sliding)
	{
		if (IsCurrentPostureSliding())
		{
			EndSlidingEvent();
		}
	}
	if (CurrentPosture == InPosture) { return CurrentPosture; }

	CurrentPosture = InPosture;
	return InPosture;
}

void UCPP_ParkourMovementComponent::UpdatePosture()
{
	if (IsObstacleWallRun())
	{
		ChangeCharacterFrictionAndYawCamera(false, false);
		return;
	}

	if (IsObstacleActioning())
	{
		ChangeCharacterFrictionAndYawCamera(false, false);
		return;
	}

	if (IsFloating())
	{
		ChangeCharacterFrictionAndYawCamera(false, true);
	}
	else
	{
		if (GetPlayerController()->GetIsCrouchInput())
		{
			UpdatePostureCrouch();
			if (IsCurrentPostureSliding())
			{

				ChangeCharacterFrictionAndYawCamera(true, false);
				return;
			}
			else
			{
				ChangeCharacterFrictionAndYawCamera(false, true);
				return;
			}
		}
		else
		{
			ChangePostureStand();
			ChangeCharacterFrictionAndYawCamera(false, !ParkourCharacter->GetIsThirdPersonMode());
		}
	}
}

void UCPP_ParkourMovementComponent::ChangePostureStand()
{
	ParkourCharacter->UnCrouch();

	// ??????????????????????????????????????????????????
	if (IsCrouching())
	{
		// ?X???C?f?B???O????
		SetPosture(ECharacterPosture::Crouch);
		return;
	}

	if (CurrentPosture != ECharacterPosture::Stand)
	{
		SetPosture(ECharacterPosture::Stand);
	}
}

void UCPP_ParkourMovementComponent::BeginPosture()
{
	if (IsFloating()) { return; }

	// ????????????????
	if (!IsWithinTheSprintThreshold()) { return; }

	if (IsMidWaterSurface()) { return; }
	if (IsHighWaterSurface()) { return; }

	if (IsObstacleActioning()) { return; }
	
	if (!bIsCanSliding) { return; }

 	Sliding();
}

void UCPP_ParkourMovementComponent::Crouching()
{
	SetPosture(ECharacterPosture::Crouch);
	CrouchedHalfHeight = CrouchedCollisionHalfHeight;
	ParkourCharacter->Crouch();
}

void UCPP_ParkourMovementComponent::UpdatePostureCrouch()
{
	// ???????????????????I??????????
	if (IsHighWaterSurface())
	{
		ChangePostureStand();
		return;
	}

	// ???????x???????????????????????I????????????????
	if (IsMidWaterSurface())
	{
		SetPosture(ECharacterPosture::Crouch);
		return;
	}

	// ???????A?N?V????????????????????
	if (IsObstacleActioning()) { return; }

	// ?X???C?f?B???O???????????????????X???C?f?B???O???I???????l??????????
	if (IsCurrentPostureSliding())
	{
		if (IsWithinTheSlidingThreshold())
		{
			return;
		}
		else
		{
			// ?R???W?????T?C?Y???X???????????UUnCrouch(????Collision?T?C?Y???????????????@????????????????)
			ParkourCharacter->UnCrouch();
		}
	}

	// ???????????????????????????X???C?f?B???O?????A???????x???????????????????X???C?f?B???O??????????
	Crouching();
}

void UCPP_ParkourMovementComponent::BeginWallRun()
{
   	if (!bIsCanWallRun) { return; }

	JumpContinuousUseCount = 0;

  	ChangeWallRunParameter(SetWallRunState(ECharacterWallRunState::Running));
	MaxWalkSpeed = WallRunMoveSpeed;

	SetObstacleAction(ECharacterObstacleAction::WallRun);

	ACPP_PlayerController* const PlayerController = GetPlayerController();
	if (PlayerController == nullptr) { return; }

	PlayerController->ResetCameraHorizonView();
	PlayerController->ResetCameraVerticalView();

	UE_LOG(ParkourMovementCompLog, Log, TEXT("BeginWallRun()"));

	//?J?????X???J?n
}

void UCPP_ParkourMovementComponent::UpdateWallRun(bool& OutIsStop)
{
	OutIsStop = false;

	if (!bIsCanWallRun) { return; }

   	const FVector StartTraceLocation = ParkourCharacter->GetWallRunTraceLocation()->GetComponentLocation();
	constexpr float FORWARD_TRACE_DISTANCE = 70.f;
	const FVector EndForwardTraceLocation = StartTraceLocation + (ParkourCharacter->GetWallRunTraceLocation()->GetForwardVector() * FORWARD_TRACE_DISTANCE);
	FHitResult OutHitForwardResult;
	if (SingleLineTraceForObstacleAction(StartTraceLocation, EndForwardTraceLocation, OutHitForwardResult))
	{
		ParkourCharacter->EndWallRunForBlueprint(ECharacterWallRunRelaseType::Fall);		
		OutIsStop = true;
		return;
	}

	constexpr float SIDE_TRACE_DISTANCE = 120.f;
	const FVector EndSideTraceCross = FVector::CrossProduct(CurrentWallRunDirection, GetWallRunSideDirectionVector()) * SIDE_TRACE_DISTANCE;
	const FVector EndSideTraceLocation = FVector(EndSideTraceCross.X, EndSideTraceCross.Y, CurrentWallRunDirection.Z) + StartTraceLocation;
	FHitResult OutHitSideResult;
	if (!SingleLineTraceForObstacleAction(StartTraceLocation, EndSideTraceLocation, OutHitSideResult))
	{
		ParkourCharacter->EndWallRunForBlueprint(ECharacterWallRunRelaseType::Fall);
		OutIsStop = true;
		return;
	}
	else
	{
		SetGroundSurfaceTypeForHitResult(OutHitSideResult);
	}

	ECharacterWallRunSide OutWallRunSide;
	FVector OutWallRunDirection;
	FindWallRunDirectionAndSide(OutHitSideResult.ImpactNormal, OutWallRunSide, OutWallRunDirection);
	if (CurrentWallRunSide != OutWallRunSide)
	{
		ParkourCharacter->EndWallRunForBlueprint(ECharacterWallRunRelaseType::Fall);
		OutIsStop = true;
		return;
	}

	CurrentWallRunDirection = OutWallRunDirection;
	const FVector WallRunMoveVelocity = CurrentWallRunDirection * GetMaxSpeed();
	const FVector NewMoveVelocity = FVector(WallRunMoveVelocity.X, WallRunMoveVelocity.Y, 0.f);
	Velocity = NewMoveVelocity;

	//UE_LOG(ParkourMovementCompLog, Log, TEXT("CurrentWallRunVelocity:%f"),Velocity.Size());

	const FRotator RotFromX = FRotationMatrix::MakeFromX(CurrentWallRunDirection).Rotator();
	constexpr float LERP_ALPHA = 0.1f;
	const FRotator TargetRotation = UKismetMathLibrary::RLerp(ParkourCharacter->GetActorRotation(), RotFromX, LERP_ALPHA, true);
	ParkourCharacter->SetActorRotation(TargetRotation, ETeleportType::TeleportPhysics);
}

ECharacterObstacleAction UCPP_ParkourMovementComponent::SetObstacleAction(ECharacterObstacleAction InObstacleAction)
{
	if (IsCurrentPostureSliding())
	{
		EndSlidingEvent();
	}

	// ????????????
	//ParkourCharacter->UnCrouch();
	//SetPosture(ECharacterPosture::Stand);

	CurrentObstacleAction = InObstacleAction;

	GetPlayerController()->SetIsCrouchPermission(!IsObstacleActioning());

	return InObstacleAction;
}

void UCPP_ParkourMovementComponent::BeginReleaseWallRun()
{
	ChangeWallRunParameter(SetWallRunState(ECharacterWallRunState::Release));
}

void UCPP_ParkourMovementComponent::UpdateReleaseWallRun(bool& OutIsStop)
{
	OutIsStop = false;

	const float NextWalkSpeed = MaxWalkSpeed - (GetWorld()->GetDeltaSeconds() * WallRunReleaseDeceleration);
	MaxWalkSpeed = FMath::Clamp(NextWalkSpeed, WallRunThreshold, WallRunMoveSpeed);
	if (NextWalkSpeed > WallRunThreshold) { return; }

	ChangeWallRunParameter(SetWallRunState(ECharacterWallRunState::Ready));

	OutIsStop = true;
}

void UCPP_ParkourMovementComponent::EndWallRun(ECharacterWallRunRelaseType InWallRunReleaseType)
{
	constexpr int32 JUMP_RECOVERY_FALL = 1;
	int32 JumpLife;
	switch (InWallRunReleaseType)
	{
	case ECharacterWallRunRelaseType::Fall:
		JumpLife = JUMP_RECOVERY_FALL;
		break;

	default:
		JumpLife = MaxJumpLife;
		break;
	}

	ResetJump(JumpLife);

	ChangeWallRunParameter(SetWallRunState(ECharacterWallRunState::Ready));
	SetObstacleAction(ECharacterObstacleAction::None);
	MaxWalkSpeed = SprintMoveSpeed;

	BeforeWallRunReleaseType = InWallRunReleaseType;

	UE_LOG(ParkourMovementCompLog, Log, TEXT("EndWallRun()"));

	//?J?????X???I??
}

bool UCPP_ParkourMovementComponent::CalculateObstacleAction(ECharacterObstacleAction& OutObstacleAction, FVector& OutActioningLocation)
{
	OutObstacleAction = ECharacterObstacleAction::None;
	OutActioningLocation = GetActorLocation();
	SetObstacleAction(OutObstacleAction);

	// ?O???????????????????????A?p???????????????O?A?W?????v?L?[?????????????????A?????????????A?N?V????????????????????
	if (!ParkourCharacter->IsMovingForward() || !IsCurrentPostureStand() || !GetPlayerController()->GetIsJumpingInput() || IsObstacleActioning() || !IsMovingOnGround())
	{
		return false;
	}

	// ?O???????Q?????????????????m?F
	const FVector ObstacleTraceStart = ParkourCharacter->GetObstacleTraceLocation()->GetComponentLocation();
	constexpr float OBSTALCE_TRACE_DISTANCE = 250.f;
	const FVector ObstacleTraceEnd = ObstacleTraceStart + (ParkourCharacter->GetObstacleTraceLocation()->GetForwardVector() * OBSTALCE_TRACE_DISTANCE);
	FHitResult ObstacleTraceHit;
	if (!SingleLineTraceForObstacleAction(ObstacleTraceStart, ObstacleTraceEnd, ObstacleTraceHit))
	{
		return false;
	}

	// ObstacleActor????????????Interface???A?N?Z?X????
	ACPP_TraceObstacleTriggerBase* TraceObstacleTrigger = Cast<ACPP_TraceObstacleTriggerBase>(ObstacleTraceHit.GetActor());
	if (TraceObstacleTrigger != nullptr)
	{
		ICPP_TraceEventTriggerInterface* TraceEventTrigger = Cast<ICPP_TraceEventTriggerInterface>(TraceObstacleTrigger);
		if (TraceEventTrigger != nullptr)
		{
			TraceEventTrigger->OnTraceEvent_Implementation(ObstacleTraceHit);
			return false;
		}
		return false;
	}

	if (!bIsCanFreeParkour) { return false; }

	// ???Q??????????????????
	const FVector WallLocation = ObstacleTraceHit.Location;
	const FVector WallNormal = ObstacleTraceHit.Normal;
	// ???Q?????O????????
	const FVector WallForward = FRotationMatrix(WallNormal.Rotation()).GetScaledAxis(EAxis::X);
	// ?????????O????????
	constexpr float WALL_FORWARD_DISTANCE = -10.f;
	const FVector WallStartForwardLocation = WallForward * WALL_FORWARD_DISTANCE;
	// ?????????C???~??????
	constexpr float WALL_HEIGHT_DISTANCE = 200.f;
	const FVector WallTraceStartLocation = (WallLocation + WallStartForwardLocation) + FVector(0.f, 0.f, WALL_HEIGHT_DISTANCE);
	const FVector WallTraceEndLocation = WallLocation + WallStartForwardLocation;
	FHitResult WallTraceHit;
	// ????????????????????????????????????
	if (!SingleLineTraceForObstacleAction(WallTraceStartLocation, WallTraceEndLocation, WallTraceHit))
	{
		return false;
	}

	// ???Q????????????????
	constexpr float VAULTABLE_DISTANCE = -150.f;
	// ?????z???A?N?V??????????????????
	const FVector BackWallTraceForward = WallForward * VAULTABLE_DISTANCE;
	const FVector BackWallTraceStartLocation = (WallLocation + BackWallTraceForward) + FVector(0.f, 0.f, WALL_HEIGHT_DISTANCE);
	const FVector BackWallTraceEndLocation = WallLocation + BackWallTraceForward;
	FHitResult BackWallHit;
	// ?????z???A?N?V??????????????????????????????
	const bool bIsBackWallHit = SingleLineTraceForObstacleAction(BackWallTraceStartLocation, BackWallTraceEndLocation, BackWallHit);

	// ???????[?g?????Q???????????m?F
	// ???Q?????????F????????????????
	constexpr float ADD_HEIGHT = 50.f;
	const FVector WallHeightLocation = WallTraceHit.Location;
	const FVector DestinationRootTraceStartLocation = WallHeightLocation + FVector(0.f, 0.f, ADD_HEIGHT);
	const FVector DestinationRootTraceForwardLocation = WallLocation + BackWallTraceForward;
	const FVector DestinationRootTraceEndLocation = FVector(DestinationRootTraceForwardLocation.X, DestinationRootTraceForwardLocation.Y, WallHeightLocation.Z + ADD_HEIGHT);
	FHitResult DestinationRootHit;
	// ?????z?????????????????Q?????????????????m?F????
	const bool bIsDestinationHit = SingleLineTraceForObstacleAction(DestinationRootTraceStartLocation, DestinationRootTraceEndLocation, DestinationRootHit);

	// ???I?n???????Q?????m?????A???????????Q??????????????
	if (!bIsDestinationHit && !bIsBackWallHit)
	{
		bool bIsCanVaultable = WallHeightLocation.Z <= ParkourCharacter->GetVaultableLocation()->GetComponentLocation().Z;
		// ???Q?????????z?????\??????		
		if (bIsCanVaultable)
		{
			constexpr float MIN_VAULT_MOVE_VELOCITY_RATE = 0.f;
			constexpr float MAX_VAULT_MOVE_VELOCITY_RATE = 1.f;
			float velocityRate = VaultMoveMaxAmountVelocityThreshold <= 0.f ? MAX_VAULT_MOVE_VELOCITY_RATE : GetOriginVelocityLength() / VaultMoveMaxAmountVelocityThreshold;
			const float currentVelocityRate = FMath::Clamp(velocityRate, MIN_VAULT_MOVE_VELOCITY_RATE, MAX_VAULT_MOVE_VELOCITY_RATE);

			// ?????????????????X?^?b?N?????????????????????????????????g???[?X
			UCapsuleComponent* MyCharacterCollision = ParkourCharacter->GetCapsuleComponent();
			FHitResult VaultTraceHitResult;
			constexpr float VaultedSafeStuckAreaLocationAddZ = 80.f;

			// ?X?^?b?N?`?F?b?N?????W???e???????p?????[?^?[?????????W???v?Z????
			FVector VaultedSafeStuckAreaLocation = BackWallTraceEndLocation + ((WallForward * VaultMoveAddDistance) * currentVelocityRate);
			// Z???????????????????????????????????????A???G??????????????????????????
			VaultedSafeStuckAreaLocation.Z += VaultedSafeStuckAreaLocationAddZ;
			if (CapsuleTraceForObstacleAction(VaultedSafeStuckAreaLocation, VaultedSafeStuckAreaLocation, MyCharacterCollision->GetScaledCapsuleRadius(), MyCharacterCollision->GetScaledCapsuleHalfHeight(), VaultTraceHitResult))
			{
				return false;
			}

			OutObstacleAction = ECharacterObstacleAction::Vault;
			constexpr float VAULT_ADD_HEIGHT = 60.f;

			OutActioningLocation = VaultedSafeStuckAreaLocation;
			SetObstacleAction(OutObstacleAction);
			ResetContinuousUseSlidingCount();
			return true;
		}
	}

	bool bIsClimingWithinRange = false;
	//???Q?????m???????????????????Q??????
	if (bIsDestinationHit || bIsBackWallHit)
	{
		const FVector DestinationRootHitLocation = DestinationRootHit.Location;
		const float ClimingRadius = FVector(WallHeightLocation.X, WallHeightLocation.Y, 0.f).Size();
		constexpr float PERMISSION_CLIMING = 30.f;

		// ?o??????????????????????????????
		if (ClimingRadius <= PERMISSION_CLIMING)
		{
			return false;
		}

		bIsClimingWithinRange = true;
	}

	// ?o??????????????????
	if (bIsClimingWithinRange)
	{
		const float CharacterClimbHeight = ParkourCharacter->GetClimbableLocation()->GetComponentLocation().Z;
		constexpr float CharacterLowClimbHeight = 125.f;

		// ???m???????????????o???????????????????n?o??
		if (CharacterLowClimbHeight > WallHeightLocation.Z)
		{
			OutObstacleAction = ECharacterObstacleAction::LowClamb;
		}
		// ???m???????????????o???????????????o??
		else if (CharacterClimbHeight > WallHeightLocation.Z)
		{
			OutObstacleAction = ECharacterObstacleAction::Clamb;
		}
		
		constexpr float CLIMB_ADD_HEIGHT = 100.f;
		OutActioningLocation = WallHeightLocation + FVector(0.f, 0.f, CLIMB_ADD_HEIGHT);
		SetObstacleAction(OutObstacleAction);
		ResetContinuousUseSlidingCount();
		return true;

	}
	return false;
}

bool UCPP_ParkourMovementComponent::CalculateFlyClimbAction(ECharacterObstacleAction& OutObstacleAction, FVector& OutActioningLocation)
{
	OutObstacleAction = ECharacterObstacleAction::None;
	OutActioningLocation = GetActorLocation();

	// ?O???????Q?????????????????m?F
	const FVector ObstacleTraceStart = ParkourCharacter->GetHeadRootComponent()->GetComponentLocation();
	constexpr float OBSTALCE_TRACE_DISTANCE = 50.f;
	const FVector ObstacleTraceEnd = ObstacleTraceStart + (ParkourCharacter->GetHeadRootComponent()->GetForwardVector() * OBSTALCE_TRACE_DISTANCE);
	FHitResult ObstacleTraceHit;
	if (!SingleLineTraceForObstacleAction(ObstacleTraceStart, ObstacleTraceEnd, ObstacleTraceHit))
	{
		return false;
	}

	// ???Q??????????????????
	const FVector WallLocation = ObstacleTraceHit.Location;
	const FVector WallNormal = ObstacleTraceHit.Normal;
	// ???Q?????O????????
	const FVector WallForward = FRotationMatrix(WallNormal.Rotation()).GetScaledAxis(EAxis::X);
	// ?????????O????????
	constexpr float WALL_FORWARD_DISTANCE = -10.f;
	const FVector WallStartForwardLocation = WallForward * WALL_FORWARD_DISTANCE;
	// ?????????C???~??????
	constexpr float WALL_HEIGHT_DISTANCE = 200.f;
	const FVector WallTraceStartLocation = (WallLocation + WallStartForwardLocation) + FVector(0.f, 0.f, WALL_HEIGHT_DISTANCE);
	const FVector WallTraceEndLocation = WallLocation + WallStartForwardLocation;
	FHitResult WallTraceHit;
	// ????????????????????????????????????
	if (!SingleLineTraceForObstacleAction(WallTraceStartLocation, WallTraceEndLocation, WallTraceHit))
	{
		return false;
	}

	// ?o????????????????
	const FVector WallHeightLocation = WallTraceHit.Location;
	const FVector WallHeightDistance = WallHeightLocation - WallLocation;
	auto ClambLocation = ParkourCharacter->GetFlyClimbableLocation()->GetComponentLocation();

	if (WallHeightLocation.Z <= ParkourCharacter->GetFlyClimbableLocation()->GetComponentLocation().Z)
	{
		OutObstacleAction = ECharacterObstacleAction::Clamb;
		constexpr float CLIMB_ADD_HEIGHT = 100.f;
		OutActioningLocation = WallHeightLocation + FVector(0.f, 0.f, CLIMB_ADD_HEIGHT);
		SetObstacleAction(OutObstacleAction);
		ResetContinuousUseSlidingCount();
		return true;
	}

	return false;
}

bool UCPP_ParkourMovementComponent::CheckGroundSurfaceType(EPhysicalSurface& OutSurfaceType)
{
	if (IsObstacleWallRun()) { return false; }
	return Super::CheckGroundSurfaceType(OutSurfaceType);
}

void UCPP_ParkourMovementComponent::ChangeCharacterFrictionAndYawCamera(bool InSeparateBreaking, bool InControllerRotYaw)
{
	// ?L?????N?^?[???n?????????C
	bUseSeparateBrakingFriction = InSeparateBreaking;

	// ?J???????????L?????N?^?[?????]
	ParkourCharacter->bUseControllerRotationYaw = InControllerRotYaw;
}

void UCPP_ParkourMovementComponent::LimitHorizontalMove()
{
	if (IsMovingOnGround()) { return; }
	const float VelocityLength = GetHorizontalVelocityLength();

	if (VelocityLength < HorizontalLimitVelocity) { return; }

	const float Rate = VelocityLength / HorizontalLimitVelocity;
	const FVector2D LimitedVelocity = GetHorizontalVelocity() / Rate;
	SetHorizontalVelocity(LimitedVelocity);
}

void UCPP_ParkourMovementComponent::ChangeWallRunParameter(ECharacterWallRunState InWallRunState)
{
	switch (InWallRunState)
	{
	case ECharacterWallRunState::Ready:
		GravityScale = DefaultGravityScale;
		MaxWalkSpeed = SprintMoveSpeed;
		break;

	case ECharacterWallRunState::Running:
		GravityScale = WallRunGravityScale;
		MaxWalkSpeed = WallRunMoveSpeed;
		break;

	case ECharacterWallRunState::Release:
		GravityScale = WallRunReleaseGravity;
		break;

	default:
		break;
	}
}

void UCPP_ParkourMovementComponent::FindWallRunDirectionAndSide(FVector InHitLocation, ECharacterWallRunSide& OutWallRunSide, FVector& OutDirection)
{
	float Dot = FVector2D::DotProduct((FVector2D)InHitLocation, (FVector2D)ParkourCharacter->GetActorRightVector());
	CurrentWallRunDirectionMagnitude = Dot;

	constexpr float LEFT = 0.f;
 	if (Dot > LEFT)
	{
		CurrentWallRunSide = ECharacterWallRunSide::Right;
	}
	else
	{
		CurrentWallRunSide = ECharacterWallRunSide::Left;
	}

	OutWallRunSide = CurrentWallRunSide;
	OutDirection = FVector::CrossProduct(InHitLocation, GetWallRunSideDirectionVector());
}

// Utility?I??????????????
bool UCPP_ParkourMovementComponent::SingleLineTraceForObstacleAction(FVector InTraceStartLocation, FVector InTraceEndLocation, FHitResult& OutHitResult)
{
	UWorld* CacheWorld = GetWorld();
	const TArray<AActor*> OutActor;
	return UKismetSystemLibrary::LineTraceSingle(CacheWorld, InTraceStartLocation, InTraceEndLocation, ETraceTypeQuery::TraceTypeQuery1, false, OutActor, EDrawDebugTrace::Type::ForDuration, OutHitResult, true);
}

// Utility?I??????????????
bool UCPP_ParkourMovementComponent::SphereTraceForObstacleAction(FVector InTraceStartLocation, FVector InTraceEndLocation, float InRadius, FHitResult& OutHitResult)
{
	UWorld* CacheWorld = GetWorld();
	const TArray<AActor*> OutActor;
	return UKismetSystemLibrary::SphereTraceSingle(CacheWorld, InTraceStartLocation, InTraceEndLocation, InRadius, ETraceTypeQuery::TraceTypeQuery1, false, OutActor, EDrawDebugTrace::Type::ForDuration, OutHitResult,true);
}

bool UCPP_ParkourMovementComponent::CapsuleTraceForObstacleAction(FVector InTraceStartLocation, FVector InTraceEndLocation, float InRadius, float InHalfHeight, FHitResult& OutHitResult)
{
	UWorld* CacheWorld = GetWorld();
	const TArray<AActor*> OutActor;
	return UKismetSystemLibrary::CapsuleTraceSingle(CacheWorld, InTraceStartLocation, InTraceEndLocation, InRadius, InHalfHeight, ETraceTypeQuery::TraceTypeQuery1, false, OutActor, EDrawDebugTrace::Type::ForDuration, OutHitResult, true);
}

bool UCPP_ParkourMovementComponent::IsMidWaterSurface()
{
	if (!IsWaterSurface()) { return false; }
	if (CurrentHitSurfaceTracePoint.Z > ParkourCharacter->GetFootRootComponent()->GetComponentLocation().Z) { return true; }
	return false;
}

bool UCPP_ParkourMovementComponent::IsHighWaterSurface()
{
	if (!IsWaterSurface()) { return false; }
	if (CurrentHitSurfaceTracePoint.Z > ParkourCharacter->GetHighWaterPointComponent()->GetComponentLocation().Z) { return true; }

	return false;
}

bool UCPP_ParkourMovementComponent::IsWallRunStartPermission()
{
	if (IsObstacleActioning()) { return false; }
	if (IsMovingOnGround()) { return false; }

	// ?n???????????????????`?F?b?N
	constexpr float TRACE_GROUND = 170.f;
	const FVector TraceStartLocation = GetActorLocation();
	const FVector TraceGroundEndLocation = TraceStartLocation + (-GetOwner()->GetActorUpVector() * TRACE_GROUND);
	FHitResult OutGroundHitResult;
	if (SingleLineTraceForObstacleAction(GetActorLocation(), TraceGroundEndLocation, OutGroundHitResult))
	{
		return false;
	}

	// ?O???????????????`?F?b?N
      	constexpr float TRACE_FORWARD = 50.f;
	const FVector TraceForwardWallEndLocation = TraceStartLocation + (GetOwner()->GetActorForwardVector() * TRACE_FORWARD);
	FHitResult OutForwardWallHitResult;
	if (SingleLineTraceForObstacleAction(TraceStartLocation, TraceForwardWallEndLocation, OutForwardWallHitResult))
	{
		return false;
	}

	//if (!ParkourCharacter->GetIsJumpingInput() && GetCurrentJumpLife() < MaxJumpLife)
	//{
	//	return false;
	//}

	return true;
}

ACPP_PlayerController* UCPP_ParkourMovementComponent::GetPlayerController()
{
	const APawn* Owner = GetPawnOwner();
	if (Owner == nullptr) { return nullptr; }

	AController* const Controller = Owner->GetController();
	if (Controller == nullptr) { return nullptr; }

	ACPP_PlayerController* const PlayerController = Cast<ACPP_PlayerController>(Controller);
	if (PlayerController == nullptr) { return nullptr; }

	return PlayerController;
}

#pragma optimize ("",on)