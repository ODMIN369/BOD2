// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//#include "Engine/EngineTypes.h"
#include "Animation/AnimInstance.h"
#include "Chaos/ChaosEngineInterface.h"

#include "GameFramework/Character.h"

#include "CPP_CharacterAnimInterface.h"
#include "CPP_CharacterAnimBase.generated.h"

/**
 * 
 */
UCLASS()
class O369_HORROR01_API UCPP_CharacterAnimBase : public UAnimInstance,public ICPP_CharacterAnimInterface
{
	GENERATED_BODY()
protected:
# pragma region UE4_Default

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

#pragma endregion

protected:
#pragma region Cache
	UPROPERTY()
	class ACPP_CharacterBase* AnimCharacter;

#pragma endregion

#pragma region SurfaceType
	EPhysicalSurface CurrentSurfaceType;

	UFUNCTION(BlueprintPure)
		EPhysicalSurface GetCurrentSurfaceType() { return CurrentSurfaceType; }
#pragma endregion

protected:
	UPROPERTY(BlueprintReadOnly)
		bool bIsCrouch;

	UPROPERTY(BlueprintReadOnly)
		bool bIsJump;

	UPROPERTY(BlueprintReadOnly)
		float Speed;

	UPROPERTY(BlueprintReadOnly)
		float Direction;

	UPROPERTY(BlueprintReadOnly)
		bool bIsSprint;

	UPROPERTY(BlueprintReadOnly)
	bool bIsThirdPersonMode;

	UPROPERTY(BlueprintReadOnly)
		float VerticalInputAxis;

protected:
	virtual void UpdatePosture();

public:
#pragma region Interface

	void SetIsSprint(bool InIsSprint) ;
	void SetIsSprint_Implementation(bool InIsSprint) override;

	void SetIsCrouch_Implementation(bool InIsCrouch) override;

	void SetIsJump_Implementation(bool InIsJump) override;

	void SetSurfaceType(EPhysicalSurface SurfaceType);
	void SetSurfaceType_Implementation(EPhysicalSurface SurfaceType) override;
	
	void SetThirdPersonMode(bool InIsThirdPersonMode);
	void SetThirdPersonMode_Implementation(bool InIsThirdPersonMode) override;

	void SetVerticalInputAxis(float InVerticalAxis);
	void SetVerticalInputAxis_Implementation(float InVerticalAxis) override;

#pragma endregion
};
