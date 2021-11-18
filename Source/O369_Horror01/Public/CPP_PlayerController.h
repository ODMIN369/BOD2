// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPP_ParkourCharacterBase.h"
#include "CPP_PlayerController.generated.h"

struct FKey;

/**
 * 
 */
UCLASS()
class O369_HORROR01_API ACPP_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	bool bIsMovingPermission = true;

	bool bIsSprintPermission = true;
	bool bIsJumpPermission = true;
	bool bIsCrouchPermission = true;

	bool bIsCrouchInput = false;
	bool IsJumpingInput = false;
	bool IsSprintInput = false;
	bool IsWalkingInput = false;

public:

	// カメラの縦軸回転の制限を変更する
	UFUNCTION(BlueprintCallable)
	void SetCameraVerticalView(float InPitchMax, float InPitchMin);

	// カメラの縦軸の制限をデフォルトに戻す
	UFUNCTION(BlueprintCallable)
	void ResetCameraVerticalView();

	// カメラの横軸回転の制限を変更する
	UFUNCTION(BlueprintCallable)
	void SetCameraHorizonView(float InYawMax, float InYawMin);

	UFUNCTION(BlueprintCallable)
		void SetCameraHorizonFixedView(float InYawMax, float InYawMin);

	// カメラの横軸の制限をデフォルトに戻す
	UFUNCTION(BlueprintCallable)
	void ResetCameraHorizonView();



public:
	UFUNCTION(BlueprintCallable)
		void SetIsMovingPermission(bool IsInMovingPermission)
	{
		bIsMovingPermission = IsInMovingPermission;
	}

	UFUNCTION(BlueprintPure) 
		bool GetIsMovingPermission(){ return bIsMovingPermission; }

	UFUNCTION(BlueprintCallable)
	void SetIsSprintPermission(bool IsInSprintPermission, FKey Key)
	{
		bIsSprintPermission = IsInSprintPermission; 
		if (IsInSprintPermission) { return; }

		Cast<ICPP_ParkCharaControllerInterface>(GetPawn())->ReleasedSprint_Implementation(Key);
	}

	UFUNCTION(BlueprintPure)
	bool GetIsSprintPermission() { return bIsSprintPermission; }

	UFUNCTION(BlueprintCallable)
	void SetIsJumpPermission(bool IsInJumpPermission, FKey Key)
	{
		bIsJumpPermission = IsInJumpPermission; 
		if (IsInJumpPermission) { return; }

		Cast<ICPP_ParkCharaControllerInterface>(GetPawn())->ReleasedJump_Implementation(Key);
	}

	UFUNCTION(BlueprintPure)
	bool GetIsJumpPermission() { return bIsJumpPermission; }

	UFUNCTION(BlueprintCallable)
	void SetIsCrouchPermission(bool IsInCrouchPermission, FKey Key);

	void SetIsCrouchPermission(bool IsInCrouchPermission);

	UFUNCTION(BlueprintPure)
	bool GetIsCrouchPermission() { return bIsCrouchPermission; }

public:
	UFUNCTION(BlueprintCallable)
		void SetIsCrouchInput(bool InIsCrouchInput) { bIsCrouchInput = InIsCrouchInput; }

	UFUNCTION(BlueprintPure)
		bool GetIsCrouchInput() { return bIsCrouchInput; }

	UFUNCTION(BlueprintCallable)
	void SetIsJumpingInput(bool InIsJumpingInput) { IsJumpingInput = InIsJumpingInput; }

	UFUNCTION(BlueprintPure)
	bool GetIsJumpingInput() { return IsJumpingInput; }

	UFUNCTION(BlueprintCallable)
	void SetIsSprintInput(bool InIsSprintInput) { IsSprintInput = InIsSprintInput; }

	UFUNCTION(BlueprintPure)
	bool GetIsSprintInput() { return IsSprintInput; }

	UFUNCTION(BlueprintPure)
		bool GetIsInputEnable() { return bCinemaDisableInputMove == false && bCinemaDisableInputLook == false; }
};
