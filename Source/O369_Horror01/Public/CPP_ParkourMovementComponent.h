// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CPP_CharacterEnum.h"

#include "CPP_CharacterMovementComponent.h"
#include "CPP_ParkourMovementComponent.generated.h"

class ACPP_ParkourCharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlidingDelegateEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLandedDelegateEvent);

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class O369_HORROR01_API UCPP_ParkourMovementComponent : public UCPP_CharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	FVector AfterVelocity;

	UCPP_ParkourMovementComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region Cache
private:
	UPROPERTY()
	ACPP_ParkourCharacterBase* ParkourCharacter;

private:
	UPROPERTY()
	float DefaultGravityScale;

#pragma endregion

private:
	const FString Category_Base = TEXT("CPP_ParkourCharacterBase:");
	const FString Category_Posture = Category_Base + TEXT("Posture");
	const FString Category_ObstacleAction = Category_Base + TEXT("ObstacleAction");
	const FString Category_WallRun = Category_Base + TEXT("WallRun");
	const FString Category_GroundMove = Category_Base + TEXT("GroundMovement");
	const FString Category_Jump = Category_Base + TEXT("Jump");
	const FString Category_Other = Category_Base + TEXT("Other");
	const FString Category_GameStyle = Category_Base + TEXT("GameStyle");
	const FString Category_SlidingSound = Category_Base + TEXT("SlidingSound");
	const FString Category_ObstacleMove = Category_Base + TEXT("ObstacleMovement");
	const FString Category_Sliding = Category_Base + TEXT("Sliding");
	const FString Category_Land = Category_Base + TEXT("Land");
	const FString Category_ParkourMoveFlags = Category_Base + TEXT("ParkourMoveFlags");

#pragma region Parameter

public:
	void SetMoveSpeed(float ChangedSpeed) { MaxWalkSpeed = ChangedSpeed; }

public:
	// ���݂̎p��
	UPROPERTY()
	ECharacterPosture CurrentPosture;

	// ���݂̍s���Ă���A�N�V����
	UPROPERTY(VisibleAnywhere, Category = Category_ObstacleAction)
	ECharacterObstacleAction CurrentObstacleAction;

	UPROPERTY(BlueprintReadOnly, Category = Category_WallRun)
		ECharacterWallRunState CurrentWallRunState;

	UPROPERTY(BlueprintReadOnly, Category = Category_WallRun)
		ECharacterWallRunSide CurrentWallRunSide;

	UPROPERTY(BlueprintReadOnly, Category = Category_WallRun)
		ECharacterWallRunRelaseType BeforeWallRunReleaseType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_GroundMove)
		float CreepingMoveSpeed = 0.f;

	// �y�����n�d���Ƃ݂Ȃ�臒l
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Land)
		float SoftLandedRigidityThreshold = 0.f;

	// �y�����n�d������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Land)
		float SoftLandedRigidityTime = 0.f;

	// �d�߂̒��n�d���Ƃ݂Ȃ�臒l
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Land)
		float HeavyLandedRigidityThreshold = 0.f;
	
	// �d�߂̒��n�d������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Land)
		float HeavyLandedRigidityTime = 0.f;

	// �W�����v��̒��n�d������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Land)
		float JumpLandedRigidityTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Land)
		TSubclassOf<class UCameraShakeBase> SoftLandedRigidityCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Land)
		TSubclassOf<class UCameraShakeBase> HeavyLandedRigidityCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_GroundMove)
		float WaterMoveSpeedRate = 1.f;
	float WaterWalkMoveSpeed = 0.f;
	float WaterJogingMoveSpeed = 0.f;
	float WaterSprintMoveSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_GroundMove)
		bool bIsSprintForwardOnly = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_ObstacleMove)
		float LadderClimbSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_ObstacleMove)
		float CliffMoveSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_ObstacleMove)
		float CreviceMoveSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_ObstacleMove)
		float BalanceBeamMoveSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_ObstacleAction)
		float ObstaclePushPower = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_WallRun)
		float WallRunMoveSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_WallRun)
		float WallRunGravityScale = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_WallRun)
		float WallRunReleaseDeceleration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_WallRun)
		float WallRunReleaseGravity = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_WallRun)
		float WallJumpUpPower = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_WallRun)
		float WallJumpSidePower = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_WallRun)
		float WallJumpForwardPower = 0.f;

	// �X���C�f�B���O���̒ǉ����x
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Sliding)
		float SlidingBoostPower = 0.f;

	// �X���C�f�B���O���Ƃ݂Ȃ�臒l
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Sliding)
		float SlidingThreshold = 0.f;

	// �X���C�f�B���O���̍Œ�ۏᑬ�x
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Sliding)
		float SlidingMinPower = 0.f;

	// �X���C�f�B���O���ɂ�������͂̍ō��l
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Sliding)
		float SlidingMaxPower = 0.f;

	// �X���C�f�B���O���̌����l
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Sliding)
		float SlidingDecelerateValue = 0.f;
	int32 SlidingContinuousUseCount = 0;

	// �X���C�f�B���O�̌�����ԉ����܂ł̃N�[���^�C��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Sliding)
		float SlidingDecelerateCoolTime = 0.f;

	// �X���C�f�B���O�̌�����ԉ����܂ł̃N�[���^�C��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Sliding)
		float BrakingDecelerationSliding = 0.f;

	// �X���C�f�B���O���ɂ�������͂̍ō��l��ON/OFF
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Sliding)
		bool bIsLimitSlidingMaxPower = false;

	// ����Ƃ݂Ȃ�臒l
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_ObstacleAction)
		float SprintThreshold = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_ObstacleAction)
		float WallRunThreshold = 0.f;

	// �{���g�A�N�V�������ɒǉ��őO�i���鋗��(0 = �ǉ��ړ��Ȃ�)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_ObstacleAction)
		float VaultMoveAddDistance = 0.f;

	// �{���g�A�N�V�������ɍő勗���O�i���邽�߂ɕK�v��Velocity��臒l
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_ObstacleAction)
		float VaultMoveMaxAmountVelocityThreshold = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Jump)
		int32 MaxJumpLife;

	int32 CurrentJumpLife;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Jump)
		float MinimalJumpPower = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Jump)
		float JumpingAddForwardPower = 0.f;

	// �A���W�����v���̃W�����v�͌����l
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Jump)
		float JumpPowerDecelerateValue = 0.f;
		int32 JumpContinuousUseCount = 0;

	// �A���W�����v�̃W�����v�͌�����ԉ����܂ł̃N�[���^�C��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Sliding)
			float JumpPowerDecelerateCoolTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_Other)
		float HorizontalLimitVelocity = 0.f;

#pragma endregion

#pragma region ParameterGet
	public:
		UFUNCTION(BlueprintPure)
		float GetCreepingMoveSpeed() { return CreepingMoveSpeed; }

		UFUNCTION(BlueprintPure)
		float GetDefaultGravity() { return DefaultGravityScale; }
#pragma endregion

#pragma region ParkourMovementFlags
	public:
		//���̍����̏�Q�������z������A�o�����肷�邱�Ƃ��ł���悤�ɂȂ�t���O
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_ParkourMoveFlags)
		bool bIsCanFreeParkour = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_ParkourMoveFlags)
		bool bIsCanSliding = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_ParkourMoveFlags)
		bool bIsCanWallRun = false;

	// �y�߂̒��n�d����L�������邩
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_ParkourMoveFlags)
		bool bIsSoftLandedRigidityEnabled = false;

	// �d�߂̒��n�d����L�������邩
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_ParkourMoveFlags)
		bool bIsHeavyLandedRigidityEnabled = false;

	// �W�����v���n��̍d����L�������邩
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_ParkourMoveFlags)
		bool bIsJumpLandedRigidityEnabled = false;
#pragma endregion

#pragma region Timer
private:
	FTimerHandle SlidingDecelerateCoolTimeHandle;
	FTimerHandle JumpDecelerateCoolTimeHandle;
	FTimerHandle SoftLandedRigidityTimeHandle;
	FTimerHandle HeavyLandedRigidityTimeHandle;
	FTimerHandle JumpLandedRigidityTimeHandle;
#pragma endregion

#pragma region Posture
public:
	UPROPERTY(BlueprintReadOnly, Category = Category_Posture)
		float CrouchedCollisionHalfHeight = 55.f;

	UPROPERTY(BlueprintReadOnly, Category = Category_Posture)
		float SlidingCollisionHalfHeight = 0.f;

public:
	void BeginSlidingEvent();

	void EndSlidingEvent();

public:
	UFUNCTION(BlueprintCallable)
		ECharacterPosture SetPosture(ECharacterPosture InPosture);

	UFUNCTION(BlueprintPure)
		bool IsCurrentPostureSliding() { return CurrentPosture == ECharacterPosture::Sliding; }

	UFUNCTION(BlueprintPure)
		bool IsCurrentPostureCrouching() { return CurrentPosture == ECharacterPosture::Crouch; }

	UFUNCTION(BlueprintPure)
		bool IsCurrentPostureStand() { return CurrentPosture == ECharacterPosture::Stand; }
#pragma endregion

#pragma region Jump
	UFUNCTION(BlueprintPure)
		int32 GetCurrentJumpLife() { return CurrentJumpLife; }

	UFUNCTION(BlueprintPure)
		bool IsCanJump()
	{
		return CurrentJumpLife > 0 && !IsObstacleActioning() && CanEverMoveOnGround() && IsMovingOnGround();
	}

	UFUNCTION(BlueprintCallable)
		void VelocityJump();

	UFUNCTION(BlueprintCallable)
		void WallJump();

	UFUNCTION(BlueprintCallable)
		void ResetJump(int32 InChangedJump);

	UFUNCTION(BlueprintPure)
	bool IsFloating() { return !IsMovingOnGround() && !IsObstacleActioning(); }



#pragma endregion

#pragma region Landed
	public:
		UPROPERTY(BlueprintAssignable)
			FLandedDelegateEvent OnNormalLandedDelegateEvent;

		UPROPERTY(BlueprintAssignable)
			FLandedDelegateEvent OnSoftRigidityLandedDelegateEvent;

		UPROPERTY(BlueprintAssignable)
			FLandedDelegateEvent OnHeavyRigidityLandedDelegateEvent;

protected:
	UFUNCTION()
	void Landed(const FHitResult& Hit);

	UFUNCTION()
	void SoftLandedRigidity();

	UFUNCTION()
	void HeavyLandedRigidity();

	UFUNCTION()
	void JumpLandedRigidity();

#pragma region ObstascleAction

public:
	UFUNCTION(BlueprintCallable)
		void ObstacleLadderMovementInput(float InInputScale);

	UFUNCTION(BlueprintCallable)
		void ObstacleCliffMovementInput(float InInputScale);

	UFUNCTION(BlueprintCallable)
		void ObstacleCreviceMovementInput(float InInputScale);

	UFUNCTION(BlueprintCallable)
		void ObstacleBalanceBeamMovementInput(float InInputScale);

public:
	UFUNCTION(BlueprintCallable)
		ECharacterObstacleAction SetObstacleAction(ECharacterObstacleAction InObstacleAction);

	UFUNCTION(BlueprintPure)
		ECharacterObstacleAction GetCurrentObstacleAction() { return CurrentObstacleAction; }

	UFUNCTION(BlueprintPure)
		bool IsObstacleActioning() { return CurrentObstacleAction != ECharacterObstacleAction::None; }

	UFUNCTION(BlueprintPure)
		bool IsObstacleVault() { return CurrentObstacleAction == ECharacterObstacleAction::Vault; }

	UFUNCTION(BlueprintPure)
		bool IsObstacleWallRun() { return CurrentObstacleAction == ECharacterObstacleAction::WallRun; }

	UFUNCTION(BlueprintPure)
		bool IsObstacleLadder() { return CurrentObstacleAction == ECharacterObstacleAction::Ladder; }

	UFUNCTION(BlueprintPure)
		bool IsObstacleCliff() { return CurrentObstacleAction == ECharacterObstacleAction::Cliff; }

	UFUNCTION(BlueprintPure)
		bool IsObstacleClimb() { return CurrentObstacleAction == ECharacterObstacleAction::Clamb; }

	UFUNCTION(BlueprintPure)
		bool IsObstaclePush() { return CurrentObstacleAction == ECharacterObstacleAction::Push; }

	UFUNCTION(BlueprintPure)
		bool IsObstacleDoor() { return CurrentObstacleAction == ECharacterObstacleAction::Door; }

	UFUNCTION(BlueprintPure)
		bool IsObstacleBalanceBeam() { return CurrentObstacleAction == ECharacterObstacleAction::BalanceBeam; }

	UFUNCTION(BlueprintPure)
		bool IsObstacleCrevice() { return CurrentObstacleAction == ECharacterObstacleAction::Crevice; }

#pragma endregion

#pragma endregion

#pragma region Sliding
protected:
	bool IsContinuousUseSliding() { return SlidingContinuousUseCount > 0; }
	void ResetContinuousUseSlidingCount() { SlidingContinuousUseCount = 0; }

public:
	// �X���C�f�B���O�������̃C�x���g
	UPROPERTY(BlueprintAssignable)
		FSlidingDelegateEvent BeginSlidingDelegateEvent;

	// �ő呬�x�ŃX���C�f�B���O�������̃C�x���g
	UPROPERTY(BlueprintAssignable)
		FSlidingDelegateEvent FullPowerSlidingDelegateEvent;

	// �X���C�f�B���O�I����̃C�x���g
	UPROPERTY(BlueprintAssignable)
		FSlidingDelegateEvent EndSlidingDelegateEvent;

public:
	void Sliding();
	UFUNCTION()
	void LandedSliding();

#pragma endregion

#pragma region WallRun

public:
	UPROPERTY(BlueprintReadWrite)
		FVector CurrentWallRunDirection;

	UPROPERTY(BlueprintReadWrite)
		float CurrentWallRunDirectionMagnitude;

	UFUNCTION(BlueprintPure)
		bool IsCurrentWallRunLeftSize() { return CurrentWallRunSide == ECharacterWallRunSide::Left; }

	UFUNCTION(BlueprintPure)
		bool IsCurrenftWallRunRightSize() { return CurrentWallRunSide == ECharacterWallRunSide::Right; }

	UFUNCTION(BlueprintPure)
		FVector GetWallRunSideDirectionVector()
	{
		constexpr float VECTOR = 1.f;

		switch (CurrentWallRunSide)
		{
		case ECharacterWallRunSide::Left:
			return FVector(0.f, 0.f, -VECTOR);
			break;

		case ECharacterWallRunSide::Right:
			return FVector(0.f, 0.f, VECTOR);
			break;

		default:
			return FVector::ZeroVector;
			break;
		}
	}

	UFUNCTION(BlueprintCallable)
		ECharacterWallRunState SetWallRunState(ECharacterWallRunState InWallRunState)
	{
		CurrentWallRunState = InWallRunState;
		return InWallRunState;
	}

	UFUNCTION(BlueprintCallable)
		ECharacterWallRunRelaseType SetBeforeWallRunReleaseType(ECharacterWallRunRelaseType InWallRunReleaseType)
	{
		BeforeWallRunReleaseType = InWallRunReleaseType;
		return InWallRunReleaseType;
	}

	UFUNCTION(BlueprintPure)
		bool IsCurrentWallRunStateReady() { return CurrentWallRunState == ECharacterWallRunState::Ready; }

	UFUNCTION(BlueprintPure)
		bool IsCurrentWallRunStateRunning() { return CurrentWallRunState == ECharacterWallRunState::Running; }

	UFUNCTION(BlueprintPure)
		bool IsCurrentWallRunStateRelease() { return CurrentWallRunState == ECharacterWallRunState::Release; }

	UFUNCTION(BlueprintPure)
		bool IsSurfaceWallRun(FVector InSurfaceNormal)
	{
		constexpr float ReleaseWall = -0.05f;
		if (InSurfaceNormal.Z < ReleaseWall) { return false; }

		FVector Normalize = FVector(InSurfaceNormal.X, InSurfaceNormal.Y, 0.f).GetSafeNormal();
		float Angle = FMath::Acos(FVector::DotProduct(Normalize, InSurfaceNormal));

		return Angle < GetWalkableFloorAngle();
	}

	UFUNCTION(BlueprintCallable)
		void ChangeWallRunParameter(ECharacterWallRunState InWallRunState);

	UFUNCTION(BlueprintCallable)
		void FindWallRunDirectionAndSide(FVector InHitLocation, ECharacterWallRunSide& OutWallRunSide, FVector& OutDirection);

	UFUNCTION(BlueprintPure)
		bool IsWallRunStartPermission();

	UFUNCTION(BlueprintPure)
		bool GetIsCanWallRun() { return bIsCanWallRun; }

#pragma endregion

#pragma region Threshould
public:
	// �����ԂƂ݂Ȃ��Ă��鑬�x�ɒB���Ă��邩
	UFUNCTION(BlueprintPure)
		bool IsWithinTheSprintThreshold() { return GetHorizontalVelocityLength() >= SprintThreshold; }

	// �X���C�f�B���O�Ƃ݂Ȃ��Ă��鑬�x�ɒB���Ă��邩
	UFUNCTION(BlueprintPure)
		bool IsWithinTheSlidingThreshold() { return GetHorizontalVelocityLength() >= SlidingThreshold; }

#pragma endregion

#pragma region Input
private:
	float RightInput = 0.f;
	float ForwardInput = 0.f;

public:
	FORCEINLINE void SetRightInput(float InRightInput) { RightInput = InRightInput; }
	FORCEINLINE float GetRightInput() { return RightInput; }

	FORCEINLINE void SetForwardInput(float InForwardInput) { ForwardInput = InForwardInput; }
	FORCEINLINE float GetForwardInput() { return ForwardInput; }

#pragma endregion

#pragma region Utility
private:
	bool IsSoftLandedRigidity() { return FMath::Abs(Velocity.Z) > SoftLandedRigidityThreshold; }
	bool IsHeavyLandedRigidity() { return FMath::Abs(Velocity.Z) > HeavyLandedRigidityThreshold; }
	bool IsJumpLandedRigidity() { return CurrentJumpLife < MaxJumpLife; }
public:
	bool IsWaterSurface() { return CurrentGroundSurfaceType == EPhysicalSurface::SurfaceType6; }
	bool IsPubbleSurface() { return CurrentGroundSurfaceType == EPhysicalSurface::SurfaceType10; }
	bool IsMidWaterSurface();
	bool IsHighWaterSurface();

	bool SingleLineTraceForObstacleAction(FVector InTraceStartLocation, FVector InTraceEndLocation, FHitResult& OutHitResult);

		bool SphereTraceForObstacleAction(FVector InTraceStartLocation, FVector InTraceEndLocation, float InRadius, FHitResult& OutHitResult);

		bool CapsuleTraceForObstacleAction(FVector InTraceStartLocation, FVector InTraceEndLocation, float InRadius, float InHalfHeight, FHitResult& OutHitResult);


	UFUNCTION(BlueprintPure)
	class ACPP_PlayerController* GetPlayerController();

#pragma endregion

#pragma region MainMethod
public:
	void UpdatePosture();
	void UpdatePostureCrouch();
	void ChangePostureStand();
	void UpdateMaxMoveSpeed();
	void ChangeCharacterFrictionAndYawCamera(bool InSeparateBreking, bool InControllerRotYaw);
	void LimitHorizontalMove();

	UFUNCTION(BlueprintCallable)
		void BeginPosture();

	UFUNCTION(BlueprintCallable)
		void BeginWallRun();

	UFUNCTION(BlueprintCallable)
	void Crouching();

	UFUNCTION(BlueprintCallable)
		void UpdateWallRun(bool& OutIsStop);

	UFUNCTION(BlueprintCallable)
		void BeginReleaseWallRun();

	UFUNCTION(BlueprintCallable)
		void UpdateReleaseWallRun(bool& OutIsStop);

	UFUNCTION(BlueprintCallable)
		void EndWallRun(ECharacterWallRunRelaseType InWallRunReleaseType);

	UFUNCTION(BlueprintCallable)
		bool CalculateObstacleAction(ECharacterObstacleAction& OutObstacleAction, FVector& OutActioningLocation);

	UFUNCTION(BlueprintCallable)
		bool CalculateFlyClimbAction(ECharacterObstacleAction& OutObstacleAction, FVector& OutActioningLocation);

	virtual bool CheckGroundSurfaceType(EPhysicalSurface& OutSurfaceType) override;
#pragma endregion

};
