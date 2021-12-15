// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerController.h"

#pragma optimize ("",off)

void ACPP_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	ResetCameraVerticalView();
	ResetCameraHorizonView();
}

void ACPP_PlayerController::ResetCameraVerticalView()
{
	SetCameraVerticalView(DefaultMaxVerticalControlRotation, DefaultMinVerticalControlRotation);
}

void ACPP_PlayerController::SetCameraVerticalView(float InPitchMax, float InPitchMin)
{
	PlayerCameraManager->ViewPitchMax = InPitchMax;
	PlayerCameraManager->ViewPitchMin = InPitchMin;
}


void ACPP_PlayerController::ResetCameraHorizonView()
{
	// これはUE4デフォルトの数値
	constexpr float DEFAULT_CAM_VIEW_YAW = 359.999f;
	SetCameraHorizonView(DEFAULT_CAM_VIEW_YAW, 0);
}

void ACPP_PlayerController::SetCameraHorizonView(float InYawMax, float InYawMin)
{

	FRotator Rot;
	Rot = PlayerCameraManager->GetCameraRotation();
	PlayerCameraManager->ViewYawMax = Rot.Yaw + InYawMax;
	PlayerCameraManager->ViewYawMin = Rot.Yaw + InYawMin;
}

void ACPP_PlayerController::SetCameraHorizonFixedView(float InYawMax, float InYawMin)
{
	PlayerCameraManager->ViewYawMax = InYawMax;
	PlayerCameraManager->ViewYawMin = InYawMin;
}

void ACPP_PlayerController::SetIsCrouchPermission(bool IsInCrouchPermission, FKey Key)
{
	bIsCrouchPermission = IsInCrouchPermission;
	if (IsInCrouchPermission) { return; }

	// 入力判定がなければ入力リリース処理を行う
	ICPP_ParkCharaControllerInterface* const Interface = Cast<ICPP_ParkCharaControllerInterface>(GetPawn());
	if (Interface == nullptr) { return; }

	Interface->ReleasedPostureChange_Implementation(Key);

	SetIsCrouchInput(false);
}

void ACPP_PlayerController::SetIsCrouchPermission(bool IsInCrouchPermission)
{
	FKey Key = FKey();
	SetIsCrouchPermission(IsInCrouchPermission, Key);
}

#pragma optimize ("",on)