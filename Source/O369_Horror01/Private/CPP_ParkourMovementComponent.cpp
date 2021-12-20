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
	// 歩き速度の閾値
	constexpr float WALK_VELOCITY_LENGTH_THRESHOLD = 20.f;
	// 障害物に対してジャンプした際に横移動してしまうバグの対応
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

	// 水位の高い場所では強制歩き
	if (IsHighWaterSurface())
	{
		SetMoveSpeed(WaterWalkMoveSpeed);
		return;
	}

	//　しゃがみ中
	if (IsCurrentPostureCrouching())
	{
		SetMoveSpeed(CrouchMoveSpeed);
		MaxWalkSpeedCrouched = CrouchMoveSpeed;
		return;
	}


	// ローカル関数==========================================================================================================================
	// 地形による走り速度の更新
	auto LocalUpdateSprintMove = [this]()
	{
		IsMidWaterSurface() == true ? SetMoveSpeed(WaterSprintMoveSpeed) : SetMoveSpeed(SprintMoveSpeed);
	};

	// 地形によるジョギング速度の更新
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

	// 着地硬直の優先順位：高
	if (bIsHeavyLandedRigidityEnabled)
	{
		if (IsHeavyLandedRigidity())
		{
			HeavyLandedRigidity();
			OnHeavyRigidityLandedDelegateEvent.Broadcast();
			return;
		}
	}

	// 着地硬直の優先順位：中
	if (bIsSoftLandedRigidityEnabled)
	{
		if (IsSoftLandedRigidity())
		{
			SoftLandedRigidity();
			OnSoftRigidityLandedDelegateEvent.Broadcast();
			return;
		}
	}

	// 着地硬直の優先順位：低
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

	//カメラの縦回転制限
	constexpr float CAM_VERTICAL_VIEW_MAX = 45.f;
	constexpr float CAM_VERTICAL_VIEW_MIN = -20.f;
	PlayerController->SetCameraVerticalView(CAM_VERTICAL_VIEW_MAX, CAM_VERTICAL_VIEW_MIN);

	//カメラの横回転制限
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
	// (直前の速度+スライディングの加速力) - (スライディング時の減速量 * 連続スライディング回数)
	const float SlidingPower = (FMath::Clamp(BeforeVelocitySize,0.f, MaxBeforeVelocitySize) + SlidingBoostPower) - (SlidingDecelerateValue * SlidingContinuousUseCount);
	// スライディングの力に制限をかけている場合は制限値を最大値とする
	const float SlidingMaxPowerValue = bIsLimitSlidingMaxPower == true ? SlidingMaxPower : SlidingPower;
	const float ClampSlidingPower = FMath::Clamp(SlidingPower, SlidingMinPower, SlidingMaxPowerValue);
	const FVector VelocityNormal = Velocity.GetSafeNormal();
	//const FVector AfterVelocity = VelocityNormal * ClampSlidingPower;

	// 正規化された移動量 * スライディング力
	AfterVelocity = VelocityNormal * ClampSlidingPower;

	Velocity = AfterVelocity;

	//ParkourCharacter->AddMovementInput(AfterVelocity);

	UE_LOG(ParkourMovementCompLog, Log, TEXT("SlidingVelocity:%f"), SlidingPower);
	UE_LOG(ParkourMovementCompLog, Log, TEXT("LimitSlidingVelocity:%f"), MaxBeforeVelocitySize);

	// スライディング時に直前のVelocity(スライディング可能速度)を上回った速度が出ていない場合はBeginSlidingEventを実行しない
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

	//カメラの縦軸回転制限解除
	PlayerController->ResetCameraVerticalView();

	//カメラの横軸回転制限解除
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

	// しゃがみ状態を解除してもしゃがみ状態の場合は抜ける
	if (IsCrouching())
	{
		// スライディング対策
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

	// 走り状態ではない
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
	// 水位が高いなら強制的に立ち状態
	if (IsHighWaterSurface())
	{
		ChangePostureStand();
		return;
	}

	// ある程度の水位にいる場合は強制的にしゃがみにする
	if (IsMidWaterSurface())
	{
		SetPosture(ECharacterPosture::Crouch);
		return;
	}

	// なにかアクション中はしゃがめない
	if (IsObstacleActioning()) { return; }

	// スライディング状態で入ってきたらスライディングが終わる閾値まで抜ける
	if (IsCurrentPostureSliding())
	{
		if (IsWithinTheSlidingThreshold())
		{
			return;
		}
		else
		{
			// コリジョンサイズ変更のために一旦UnCrouch(直接Collisionサイズを変えられる方法を見つけたら削除)
			ParkourCharacter->UnCrouch();
		}
	}

	// 走り状態とみなされた時のみスライディングする、ある程度の水位にいる場合はスライディングができない
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

	//カメラ傾き開始
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

	// 強制立ち状態
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

	//カメラ傾き終了
}

bool UCPP_ParkourMovementComponent::CalculateObstacleAction(ECharacterObstacleAction& OutObstacleAction, FVector& OutActioningLocation)
{
	OutObstacleAction = ECharacterObstacleAction::None;
	OutActioningLocation = GetActorLocation();

	// 前方方向に入力していない、姿勢が立ち状態以外、ジャンプキーを入力していない、なにかしらのアクション中の場合は抜ける
	if (!ParkourCharacter->IsMovingForward() || !IsCurrentPostureStand() || !GetPlayerController()->GetIsJumpingInput() || IsObstacleActioning() || !IsMovingOnGround())
	{
		return false;
	}

	// 前方に障害物があるかどうか確認
	const FVector ObstacleTraceStart = ParkourCharacter->GetObstacleTraceLocation()->GetComponentLocation();
	constexpr float OBSTALCE_TRACE_DISTANCE = 250.f;
	const FVector ObstacleTraceEnd = ObstacleTraceStart + (ParkourCharacter->GetObstacleTraceLocation()->GetForwardVector() * OBSTALCE_TRACE_DISTANCE);
	FHitResult ObstacleTraceHit;
	if (!SingleLineTraceForObstacleAction(ObstacleTraceStart, ObstacleTraceEnd, ObstacleTraceHit))
	{
		return false;
	}

	// ObstacleActorだった場合はInterfaceへアクセスする
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

	// 障害物の高さを取得する
	const FVector WallLocation = ObstacleTraceHit.Location;
	const FVector WallNormal = ObstacleTraceHit.Normal;
	// 障害物の前方を取得
	const FVector WallForward = FRotationMatrix(WallNormal.Rotation()).GetScaledAxis(EAxis::X);
	// ちょっと前にずらす
	constexpr float WALL_FORWARD_DISTANCE = -10.f;
	const FVector WallStartForwardLocation = WallForward * WALL_FORWARD_DISTANCE;
	// 上からレイを降らせる
	constexpr float WALL_HEIGHT_DISTANCE = 200.f;
	const FVector WallTraceStartLocation = (WallLocation + WallStartForwardLocation) + FVector(0.f, 0.f, WALL_HEIGHT_DISTANCE);
	const FVector WallTraceEndLocation = WallLocation + WallStartForwardLocation;
	FHitResult WallTraceHit;
	// なにともぶつからなかった場合は抜ける
	if (!SingleLineTraceForObstacleAction(WallTraceStartLocation, WallTraceEndLocation, WallTraceHit))
	{
		return false;
	}

	// 障害物の分厚さを取得
	constexpr float VAULTABLE_DISTANCE = -150.f;
	// 飛び越えアクション分の厚さを基準
	const FVector BackWallTraceForward = WallForward * VAULTABLE_DISTANCE;
	const FVector BackWallTraceStartLocation = (WallLocation + BackWallTraceForward) + FVector(0.f, 0.f, WALL_HEIGHT_DISTANCE);
	const FVector BackWallTraceEndLocation = WallLocation + BackWallTraceForward;
	FHitResult BackWallHit;
	// 飛び越えアクション分以上の厚さがあるかどうか
	const bool bIsBackWallHit = SingleLineTraceForObstacleAction(BackWallTraceStartLocation, BackWallTraceEndLocation, BackWallHit);

	// 移動ルートに障害物があるか確認
	// 障害物として認識するための高さ
	constexpr float ADD_HEIGHT = 50.f;
	const FVector WallHeightLocation = WallTraceHit.Location;
	const FVector DestinationRootTraceStartLocation = WallHeightLocation + FVector(0.f, 0.f, ADD_HEIGHT);
	const FVector DestinationRootTraceForwardLocation = WallLocation + BackWallTraceForward;
	const FVector DestinationRootTraceEndLocation = FVector(DestinationRootTraceForwardLocation.X, DestinationRootTraceForwardLocation.Y, WallHeightLocation.Z + ADD_HEIGHT);
	FHitResult DestinationRootHit;
	// 飛び越え分の移動中に障害物があるかどうか確認する
	const bool bIsDestinationHit = SingleLineTraceForObstacleAction(DestinationRootTraceStartLocation, DestinationRootTraceEndLocation, DestinationRootHit);

	// 目的地まで障害物検知なし、移動先は障害物の上ではない
	if (!bIsDestinationHit && !bIsBackWallHit)
	{
		bool bIsCanVaultable = WallHeightLocation.Z <= ParkourCharacter->GetVaultableLocation()->GetComponentLocation().Z;
		// 障害物が飛び越え可能な高さ		
		if (bIsCanVaultable)
		{
			constexpr float MIN_VAULT_MOVE_VELOCITY_RATE = 0.f;
			constexpr float MAX_VAULT_MOVE_VELOCITY_RATE = 1.f;
			float velocityRate = VaultMoveMaxAmountVelocityThreshold <= 0.f ? MAX_VAULT_MOVE_VELOCITY_RATE : GetOriginVelocityLength() / VaultMoveMaxAmountVelocityThreshold;
			const float currentVelocityRate = FMath::Clamp(velocityRate, MIN_VAULT_MOVE_VELOCITY_RATE, MAX_VAULT_MOVE_VELOCITY_RATE);

			// 細い隙間といったスタックしてしまう箇所に入れなくする対策トレース
			UCapsuleComponent* MyCharacterCollision = ParkourCharacter->GetCapsuleComponent();
			FHitResult VaultTraceHitResult;
			constexpr float VaultedSafeStuckAreaLocationAddZ = 50.f;

			// スタックチェックの座標を各種補正パラメーター込みの座標で計算する
			FVector VaultedSafeStuckAreaLocation = BackWallTraceEndLocation + ((WallForward * VaultMoveAddDistance) * currentVelocityRate);
			// Z軸をそのままにすると床に埋まってしまい、接触判定になるので少し浮かせる
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
	//障害物検知もしくは移動先が障害物の上
	if (bIsDestinationHit || bIsBackWallHit)
	{
		const FVector DestinationRootHitLocation = DestinationRootHit.Location;
		const float ClimingRadius = FVector(WallHeightLocation.X, WallHeightLocation.Y, 0.f).Size();
		constexpr float PERMISSION_CLIMING = 30.f;

		// 登れるほどの足場がなければ抜ける
		if (ClimingRadius <= PERMISSION_CLIMING)
		{
			return false;
		}

		bIsClimingWithinRange = true;
	}

	bool bIsNormalCliming = false;
	// 登れる分の足場がある
	if (bIsClimingWithinRange)
	{
		// 登れる高さの場合は通常登り
		bIsNormalCliming = ParkourCharacter->GetClimbableLocation()->GetComponentLocation().Z > WallHeightLocation.Z;
	}

	// 登りアクションとして計算結果を返す
	if (bIsNormalCliming)
	{
		OutObstacleAction = ECharacterObstacleAction::Clamb;
		constexpr float CLIMB_ADD_HEIGHT = 100.f;
		OutActioningLocation = WallHeightLocation + FVector(0.f, 0.f, CLIMB_ADD_HEIGHT);
		SetObstacleAction(OutObstacleAction);
		ResetContinuousUseSlidingCount();
		return true;
	}

	OutObstacleAction = ECharacterObstacleAction::None;
	SetObstacleAction(OutObstacleAction);

	OutActioningLocation = GetActorLocation();

	return false;
}

bool UCPP_ParkourMovementComponent::CalculateFlyClimbAction(ECharacterObstacleAction& OutObstacleAction, FVector& OutActioningLocation)
{
	OutObstacleAction = ECharacterObstacleAction::None;
	OutActioningLocation = GetActorLocation();

	// 前方に障害物があるかどうか確認
	const FVector ObstacleTraceStart = ParkourCharacter->GetHeadRootComponent()->GetComponentLocation();
	constexpr float OBSTALCE_TRACE_DISTANCE = 50.f;
	const FVector ObstacleTraceEnd = ObstacleTraceStart + (ParkourCharacter->GetHeadRootComponent()->GetForwardVector() * OBSTALCE_TRACE_DISTANCE);
	FHitResult ObstacleTraceHit;
	if (!SingleLineTraceForObstacleAction(ObstacleTraceStart, ObstacleTraceEnd, ObstacleTraceHit))
	{
		return false;
	}

	// 障害物の高さを取得する
	const FVector WallLocation = ObstacleTraceHit.Location;
	const FVector WallNormal = ObstacleTraceHit.Normal;
	// 障害物の前方を取得
	const FVector WallForward = FRotationMatrix(WallNormal.Rotation()).GetScaledAxis(EAxis::X);
	// ちょっと前にずらす
	constexpr float WALL_FORWARD_DISTANCE = -10.f;
	const FVector WallStartForwardLocation = WallForward * WALL_FORWARD_DISTANCE;
	// 上からレイを降らせる
	constexpr float WALL_HEIGHT_DISTANCE = 200.f;
	const FVector WallTraceStartLocation = (WallLocation + WallStartForwardLocation) + FVector(0.f, 0.f, WALL_HEIGHT_DISTANCE);
	const FVector WallTraceEndLocation = WallLocation + WallStartForwardLocation;
	FHitResult WallTraceHit;
	// なにともぶつからなかった場合は抜ける
	if (!SingleLineTraceForObstacleAction(WallTraceStartLocation, WallTraceEndLocation, WallTraceHit))
	{
		return false;
	}

	// 登れる高さかどうか
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
	// キャラクターの地面との摩擦
	bUseSeparateBrakingFriction = InSeparateBreaking;

	// カメラによるキャラクターの回転
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

// Utility的なのに移動する
bool UCPP_ParkourMovementComponent::SingleLineTraceForObstacleAction(FVector InTraceStartLocation, FVector InTraceEndLocation, FHitResult& OutHitResult)
{
	UWorld* CacheWorld = GetWorld();
	const TArray<AActor*> OutActor;
	return UKismetSystemLibrary::LineTraceSingle(CacheWorld, InTraceStartLocation, InTraceEndLocation, ETraceTypeQuery::TraceTypeQuery1, false, OutActor, EDrawDebugTrace::Type::ForDuration, OutHitResult, true);
}

// Utility的なのに移動する
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

	// 地面に接近しているかチェック
	constexpr float TRACE_GROUND = 170.f;
	const FVector TraceStartLocation = GetActorLocation();
	const FVector TraceGroundEndLocation = TraceStartLocation + (-GetOwner()->GetActorUpVector() * TRACE_GROUND);
	FHitResult OutGroundHitResult;
	if (SingleLineTraceForObstacleAction(GetActorLocation(), TraceGroundEndLocation, OutGroundHitResult))
	{
		return false;
	}

	// 前方に壁があるかチェック
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