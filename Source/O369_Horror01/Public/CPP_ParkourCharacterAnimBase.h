// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_CharacterEnum.h"
#include "CPP_ParkourCharaAnimInterface.h"
#include "CPP_CharacterAnimBase.h"
#include "CPP_ParkourCharacterAnimBase.generated.h"

class ACPP_ParkourCharacterBase;

/**
 * 
 */
UCLASS()
class O369_HORROR01_API UCPP_ParkourCharacterAnimBase : public UCPP_CharacterAnimBase,public ICPP_ParkourCharaAnimInterface
{
	GENERATED_BODY()

private:
	ECharacterPosture CharacterPosture;
	ECharacterObstacleAction ObstacleAction;
	TWeakObjectPtr<ACPP_ParkourCharacterBase> ParkourAnimCharacter;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsSliding;

	UPROPERTY(BlueprintReadOnly)
	bool bIsVault;

	UPROPERTY(BlueprintReadOnly)
	bool bIsClimb;

	UPROPERTY(BlueprintReadOnly)
	bool bIsWallRun;

	UPROPERTY(BlueprintReadOnly)
	bool bIsCliff;

	UPROPERTY(BlueprintReadOnly)
	bool bIsHanging;

	UPROPERTY(BlueprintReadOnly)
	float CliffMoveDir;

	UPROPERTY(BlueprintReadOnly)
	float WallRunDirection;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentJump;

protected :
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	virtual void UpdatePosture() override;
	void UpdateObstacleAction();

public:
	void ChangePosture(ECharacterPosture InCharacterPosture);
	virtual void ChangePosture_Implementation(ECharacterPosture InCharacterPosture) override;

	void ChangeJumpState(bool InIsJump);
	void ChangeJumpState_Implementation(bool InIsJump) override;

	void ChangeAction(ECharacterObstacleAction InAction);
	void ChangeAction_Implementation(ECharacterObstacleAction InAction) override;

	void SetWallRunDirection(float InDirectionMagnitude);
	void SetWallRunDirection_Implementation(float InDirectionMagnitude) override;

	void SetCliffMoveDirection(float CriffMoveDir);
	void SetCliffMoveDirection_Implementation(float InCriffMoveDir);

	void SetCurrentJump(int32 InJumpLife);
	void SetCurrentJump_Implementation(int32 InJumpLife) override;
	
};
