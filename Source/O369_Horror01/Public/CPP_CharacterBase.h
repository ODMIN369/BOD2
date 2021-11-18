// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/World.h"
#include "Engine/EngineTypes.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "CPP_CharacterMovementComponent.h"


#include "CPP_CharacterAnimBase.h"
#include "CPP_CharacterBase.generated.h"

class UCPP_TransformComponent;
class USpringArmComponent;
class UCameraComponent;
class USoundBase;

UCLASS(BlueprintType)
class O369_HORROR01_API ACPP_CharacterBase : public ACharacter
{
	GENERATED_BODY()

# pragma region UE4_Default
public:
	// Sets default values for this character's properties
	ACPP_CharacterBase(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* InPlayerInputComponent) override;
#pragma endregion

private:
#pragma region Cache

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCPP_CharacterMovementComponent* OriginCharacterMovement;

#pragma endregion

protected:
#pragma region Component

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCPP_TransformComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCPP_TransformComponent* BodyRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCPP_TransformComponent* HeadRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCPP_TransformComponent* FootRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCPP_TransformComponent* HighWaterPointComponent;

public:

	const UCPP_TransformComponent* GetBodyRootComponent() { return BodyRootComponent; }
	const UCPP_TransformComponent* GetHeadRootComponent() { return HeadRootComponent; }
	const UCPP_TransformComponent* GetFootRootComponent() { return FootRootComponent; }
	const UCPP_TransformComponent* GetHighWaterPointComponent() { return HighWaterPointComponent; }

#pragma endregion

private:
#pragma region CollisionQuery
	FCollisionQueryParams GroundCheckQueryParams;

#pragma endregion

protected:
#pragma region AnimationInstance

	UPROPERTY(BlueprintReadOnly)
		UCPP_CharacterAnimBase* CharacterAnimInstance;

#pragma endregion

#pragma region Landed
	// ソフトリファレンス場合
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
	//	TSoftObjectPtr<class USoundBase> LandedWood;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
	//	TSoftObjectPtr<class USoundBase> LandedMetal;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
	//	TSoftObjectPtr<class USoundBase> LandedGrass;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
	//	TSoftObjectPtr<class USoundBase> LandedSand;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
	//	TSoftObjectPtr<class USoundBase> LandedWater;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
	//	TSoftObjectPtr<class USoundBase> LandedCarpet;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
	//	TSoftObjectPtr<class USoundBase> LandedRock;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
	//	TSoftObjectPtr<class USoundBase> LandedGravel;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
	//	TSoftObjectPtr<class USoundBase> LandedPubble;

	//ハードリファレンス
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
		USoundBase* LandedWood;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
		USoundBase* LandedMetalFence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
		USoundBase* LandedGrass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
		USoundBase* LandedSand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
		USoundBase* LandedWater;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
		USoundBase* LandedCarpet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
		USoundBase* LandedRock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
		USoundBase* LandedGravel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Landed")
		USoundBase* LandedPubble;
#pragma endregion

#pragma region Parameter

	float DefaultCameraFov;

	UPROPERTY(BlueprintReadOnly)
		float ThirdPersonCameraViewX = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_CharacterBase:Parameter")
		bool bIsThirdPersonMode = false;

	UFUNCTION(BlueprintCallable)
		void SetDefaultCameraFov(float Fov) { DefaultCameraFov = Fov; }

	UFUNCTION(BlueprintPure)
		float GetDefaultCameraFov() { return DefaultCameraFov; }

#pragma endregion 

#pragma region InputAxis
	float CurrentForwardInputAxis;
	float CurrentRightInputAxis;

#pragma endregion

#pragma region MovementInput
	virtual bool MovementForwardInput(float InInputScale, bool InForce);

	virtual bool MovementRightInput(float InInputScale, bool InForce);

#pragma endregion

public:
	bool IsFalling() { return OriginCharacterMovement->IsFalling(); }

#pragma region Utility
	// VectorLengthSquaredを扱いやすくした値を返す関数
	UFUNCTION(BlueprintPure)
		float GetOriginVelocityLength() { return GetVelocity().SizeSquared() / 1000.f; }

	UFUNCTION(BlueprintPure)
		float GetOriginHorizontalVelocity() { return GetHorizontalVelocity().SizeSquared() / 1000.f; }

	// BP側に公開してもOK
	FORCEINLINE FVector GetDirection(EAxis::Type Axis)
	{
		const FRotator Rot = FRotator(0.f, GetControlRotation().Yaw, 0.f);
		const FVector ForwardDir = FRotationMatrix(Rot).GetUnitAxis(Axis);

		return ForwardDir;
	}

	UFUNCTION(BlueprintPure)
		FVector2D GetHorizontalVelocity() { return (FVector2D)OriginCharacterMovement->Velocity; }

	UFUNCTION(BlueprintCallable)
		FVector SetHorizontalVelocity(FVector2D InHorizontalVelocity)
	{
		OriginCharacterMovement->Velocity.X = InHorizontalVelocity.X;
		OriginCharacterMovement->Velocity.Y = InHorizontalVelocity.Y;

		return OriginCharacterMovement->Velocity;
	}

	UFUNCTION(BlueprintPure)
		float GetCurrentDirectionOfVelocity();

	UFUNCTION(BlueprintPure)
		bool IsMovingForward() { return CurrentForwardInputAxis > 0.f; }

	UFUNCTION(BlueprintPure)
		float GetCurrentForwardInputAxis() { return CurrentForwardInputAxis; }

	UFUNCTION(BlueprintPure)
		bool IsMovingRight() { return CurrentRightInputAxis > 0.f; }

	UFUNCTION(BlueprintPure)
		bool IsMovingLeft() { return CurrentRightInputAxis < 0.f; }

	UFUNCTION(BlueprintPure)
		float GetCurrentRightInputAxis() { return CurrentRightInputAxis; }

	UFUNCTION(BlueprintPure)
		UCPP_CharacterMovementComponent* const GetOriginCharacterMovement() { return  OriginCharacterMovement; }

#pragma endregion

#pragma region GetParameter
	UFUNCTION(BlueprintPure)
		bool GetIsThirdPersonMode() { return bIsThirdPersonMode; }

#pragma endregion 

};
