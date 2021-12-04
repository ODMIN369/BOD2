// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_CharacterBase.h"
#include "CoreMinimal.h"
#include "Engine/World.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "TimerManager.h"

#include "CPP_ParkourMovementComponent.h"

#include "CPP_ParkourCharacterAnimBase.h"
#include "CPP_ParkourCharaAnimInterface.h"
#include "CPP_ParkCharaControllerInterface.h"

#include "CPP_TraceInteractiveComponent.h"

#include "CPP_CharacterEnum.h"

#include "CPP_ParkourCharacterBase.generated.h"

class UAnimMontage;
class UCPP_TransformComponent;
class UCharacterMoveComponent;
class USoundBase;
struct FKey;

USTRUCT(BlueprintType)
struct FCharacterAnimationMontageData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AnimPlayRate;

	const bool IsAnimMontageValid() { return AnimMontage != nullptr; }
};

/**
 * 
 */
UCLASS(BlueprintType)
class O369_HORROR01_API ACPP_ParkourCharacterBase : public ACPP_CharacterBase,public ICPP_ParkourCharaAnimInterface,public ICPP_ParkCharaControllerInterface
{
	GENERATED_BODY()
	
// Fill out your copyright notice in the Description page of Project Settings.
# pragma region UE4_Default
public:
	// Sets default values for this character's properties
	ACPP_ParkourCharacterBase(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Jump() override;

#pragma endregion

public:
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	 void ReleasedSprint_Implementation(FKey Key) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	 void ReleasedJump_Implementation(FKey Key) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ReleasedPostureChange_Implementation(FKey Key) override;

#pragma region Component
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCPP_TransformComponent* ObstacleActionComponentRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCPP_TransformComponent* ObstacleTraceLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCPP_TransformComponent* ClimbableLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCPP_TransformComponent* FlyClibableLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCPP_TransformComponent* VaultableLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCPP_TransformComponent* WallRunTraceLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCPP_TraceInteractiveComponent* TraceInteractiveComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCPP_TraceInteractiveComponent* TraceProgressInteractiveComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCPP_ParkourMovementComponent* ParkourCharacterMovement;

public:
	const UCPP_TransformComponent* GetObstacleActionComponentRoot(){ return ObstacleActionComponentRoot; }
	const UCPP_TransformComponent* GetObstacleTraceLocation(){ return ObstacleTraceLocation; }
	const UCPP_TransformComponent* GetClimbableLocation(){ return ClimbableLocation; }
	const UCPP_TransformComponent* GetFlyClimbableLocation() { return FlyClibableLocation; }
	const UCPP_TransformComponent* GetVaultableLocation() { return VaultableLocation; }
	const UCPP_TransformComponent* GetWallRunTraceLocation() { return WallRunTraceLocation; }
#pragma endregion

protected:
#pragma region Animation

	UPROPERTY(BlueprintReadOnly)
		UCPP_ParkourCharacterAnimBase* ParkourAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAnimMontage* LandedRigidityAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAnimMontage* LaddingEndAnimMontage;

#pragma endregion

#pragma region InputFlag

#pragma endregion

#pragma region Sprint
private:
	void UpdateIsSprint();

#pragma endregion 

#pragma region Jump

	UFUNCTION(BlueprintCallable)
		void WallJump();

	UFUNCTION(BlueprintCallable)
		void VelocityJump();

	UFUNCTION(BlueprintCallable)
		void ResetJump();

#pragma endregion

#pragma region WallRun

	UFUNCTION(BlueprintCallable)
		void ChangeWallRunParameter(ECharacterWallRunState InWallRunState);

	UFUNCTION(BlueprintCallable)
		void FindWallRunDirectionAndSide(FVector InHitLocation, ECharacterWallRunSide& OutWallRunSide, FVector& OutDirection);

#pragma endregion

#pragma region AnimMontage
public:
	float PlayLandedRigidityAnimMontage(float InPlayRate = 1.f);

	UFUNCTION(BlueprintCallable)
	float PlayLaddingEndAnimMontage(float InPlayRate = 1.f);

#pragma endregion

#pragma region MovementInput
	UFUNCTION(BlueprintCallable)
		virtual bool MovementForwardInput(float InInputScale, bool InForce) override;

	UFUNCTION(BlueprintCallable)
		virtual bool MovementRightInput(float InInputScale, bool InForce) override;


#pragma endregion

#pragma region SlidingSound
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USoundBase* SlidingSound;

#pragma endregion

#pragma region Utility
	public:
	UFUNCTION(BlueprintPure)
		 UCPP_ParkourMovementComponent* const GetParkourMovementComponent() { return ParkourCharacterMovement; }

	UFUNCTION(BlueprintPure)
		UCPP_ParkourCharacterAnimBase* const GetParkourCharacterAnimInstance() { return ParkourAnimInstance; }

#pragma endregion

#pragma region MainMethod
private:
	void UpdatePosture();
	void UpdatePostureCrouch();
	void ChangePostureStand();
	void UpdateMaxMoveSpeed();
	void LimitHorizontalMove();
	float PlayAnimationMontage(FCharacterAnimationMontageData InAnimMontageData);

	UFUNCTION(BlueprintCallable)
		void BeginPosture();

	UFUNCTION(BlueprintCallable)
		void BeginWallRun();

	UFUNCTION(BlueprintCallable)
		void UpdateWallRun(bool& OutIsStop);

	UFUNCTION(BlueprintCallable)
		void BeginReleaseWallRun();

	UFUNCTION(BlueprintCallable)
		void UpdateReleaseWallRun(bool& OutIsStop);

	UFUNCTION(BlueprintCallable)
		void EndWallRun(ECharacterWallRunRelaseType InWallRunReleaseType);


	UFUNCTION(BlueprintCallable)
		void UpdateTraceInteractive(class UCameraComponent* Camera);

	UFUNCTION(BlueprintCallable)
		void UpdateProgressTraceInteractive(class UCameraComponent* Camera);

	UFUNCTION(BlueprintCallable)
		float PlayObstacleActionAnimMontage(FCharacterAnimationMontageData InAnimMontageData);

	UFUNCTION(BlueprintCallable)
		bool CalculateObstacleAction(ECharacterObstacleAction& OutObstacleAction, FVector& OutActioningLocation);

	public:
		UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
			void BeginWallRunForBlueprint();
		UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
			void EndWallRunForBlueprint(ECharacterWallRunRelaseType InWallRunReleaseType);

#pragma endregion
};
