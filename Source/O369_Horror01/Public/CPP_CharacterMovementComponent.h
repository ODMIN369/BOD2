// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CPP_CharacterMovementComponent.generated.h"


class UCPP_TransformComponent;
class ACPP_CharacterBase;

/**
 * 
 */
UCLASS()
class O369_HORROR01_API UCPP_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UCPP_CharacterMovementComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


#pragma region Parameter
protected:
	bool bIsSprint = false;

public:
	bool GetIsSprint() { return bIsSprint; }

#pragma endregion

private:
#pragma region Cache
	UPROPERTY()
	ACPP_CharacterBase* OriginCharacterOwner;

#pragma endregion

public:
	const ACPP_CharacterBase* GetOriginCharacterOwner() { return OriginCharacterOwner; }

protected:
	// 走る速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_GroundMove)
		float SprintMoveSpeed = 0.f;

	// 歩く速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_GroundMove)
		float WalkMoveSpeed = 0.f;

	// ジョギングの速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_GroundMove)
		float JogingMoveSpeed = 0.f;

	// しゃがみ移動速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_GroundMove)
		float CrouchMoveSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Category_GroundMove)
		bool bIsJogingMode = false;

#pragma region Joging
public:
	UFUNCTION(BlueprintCallable)
		void SetIsJoging(bool InIsJoging) { bIsJogingMode = InIsJoging; }

#pragma endregion 

public:
	UFUNCTION(BlueprintPure)
		float GetSprintMoveSpeed() { return SprintMoveSpeed; }

	UFUNCTION(BlueprintPure)
		float GetWalkMoveSpeed() { return WalkMoveSpeed; }

	UFUNCTION(BlueprintPure)
		float GetJogingMoveSpeed() { return JogingMoveSpeed; }

	UFUNCTION(BlueprintPure)
		float GetCrouchMoveSpeed() { return CrouchMoveSpeed; }

public:
	// VectorLengthSquaredを扱いやすくした値を返す関数
	UFUNCTION(BlueprintPure)
		float GetOriginVelocityLength() { return GetOwner()->GetVelocity().SizeSquared() / 1000.f; }

	UFUNCTION(BlueprintPure)
		float GetHorizontalVelocityLength() { return GetHorizontalVelocity().SizeSquared() / 1000.f; }
	UFUNCTION(BlueprintPure)
		FVector2D GetHorizontalVelocity() { return (FVector2D)Velocity; }

	UFUNCTION(BlueprintCallable)
		FVector SetHorizontalVelocity(FVector2D InHorizontalVelocity)
	{
		Velocity.X = InHorizontalVelocity.X;
		Velocity.Y = InHorizontalVelocity.Y;

		return Velocity;
	}

#pragma region CollisionQuery
protected:
	FCollisionQueryParams GroundCheckQueryParams;

#pragma endregion

#pragma region GroundSurface
protected:
	EPhysicalSurface CurrentGroundSurfaceType;

	FVector CurrentHitSurfaceTracePoint;

public:
	FORCEINLINE void SetGroundSurfaceType(EPhysicalSurface InPhysicalSurface) { CurrentGroundSurfaceType = InPhysicalSurface; }

	FORCEINLINE void SetGroundSurfaceTypeForHitResult(const FHitResult& InHitResult);

	UFUNCTION(BlueprintPure)
		EPhysicalSurface GetCurrentGroundSurfaceType() { return CurrentGroundSurfaceType; }

	virtual bool CheckGroundSurfaceType(EPhysicalSurface& OutSurfaceType);
#pragma endregion
};
