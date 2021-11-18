// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_CharacterBase.h"
#include "CPP_TransformComponent.h"

#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Pawn.h"

#pragma optimize ("",off)

// 動作に関する処理は基本的にBlueprint側で呼び出しています。
// C++側は必要な初期化をするぐらい
// 将来的にUCPP_CharacterMovementComponentにする

// Sets default values
ACPP_CharacterBase::ACPP_CharacterBase(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCPP_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	Root = CreateDefaultSubobject<UCPP_TransformComponent>(TEXT("Root"));
	Root->Initialize(GetMesh());

	BodyRootComponent = CreateDefaultSubobject<UCPP_TransformComponent>(TEXT("BodyRootComponent"));
	BodyRootComponent->Initialize((USceneComponent*)Root);

	HeadRootComponent = CreateDefaultSubobject<UCPP_TransformComponent>(TEXT("HeadRootComponent"));
	HeadRootComponent->Initialize((USceneComponent*)BodyRootComponent);

	FootRootComponent = CreateDefaultSubobject<UCPP_TransformComponent>(TEXT("FootRootComponent"));
	FootRootComponent->Initialize((USceneComponent*)BodyRootComponent);

	HighWaterPointComponent = CreateDefaultSubobject<UCPP_TransformComponent>(TEXT("HighWaterPointComponent"));
	HighWaterPointComponent->Initialize((USceneComponent*)BodyRootComponent);

	SetActorHiddenInGame(false);

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	OriginCharacterMovement = Cast<UCPP_CharacterMovementComponent>(GetCharacterMovement());

	CharacterAnimInstance = Cast<UCPP_CharacterAnimBase>(GetMesh()->GetAnimInstance());
	CharacterAnimInstance->SetThirdPersonMode_Implementation(bIsThirdPersonMode);

	GroundCheckQueryParams.AddIgnoredActor(this);
	GroundCheckQueryParams.bReturnPhysicalMaterial = true;
}

void ACPP_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CharacterAnimInstance->SetSurfaceType_Implementation(OriginCharacterMovement->GetCurrentGroundSurfaceType());
	CharacterAnimInstance->SetIsCrouch_Implementation(bIsCrouched);
	CharacterAnimInstance->SetIsJump_Implementation(bIsCrouched);
}

float ACPP_CharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

bool ACPP_CharacterBase::MovementForwardInput(float InInputScale, bool InForce = false)
{
	CurrentForwardInputAxis = InInputScale;

	constexpr float NONE_INPUT = 0.f;
	if (InInputScale == NONE_INPUT) { return false; }

	AddMovementInput(GetDirection(EAxis::X), InInputScale, InForce);

	return true;
}

bool ACPP_CharacterBase::MovementRightInput(float InInputScale, bool InForce = false)
{
	CurrentRightInputAxis = InInputScale;

	constexpr float NONE_INPUT = 0.f;
	if (InInputScale == NONE_INPUT) { return false; }

	AddMovementInput(GetDirection(EAxis::Y), InInputScale, InForce);

	return true;
}

// Called to bind functionality to input
void ACPP_CharacterBase::SetupPlayerInputComponent(UInputComponent* InPlayerInputComponent)
{
	Super::SetupPlayerInputComponent(InPlayerInputComponent);
}

float ACPP_CharacterBase::GetCurrentDirectionOfVelocity()
{
	const float CurrentDirectionOfVelocity = FVector::DotProduct(OriginCharacterMovement->Velocity, GetActorRightVector());
	constexpr float VELOCITY_LEFT = -400.f;
	constexpr float VELOCITY_RIGHT = 400.f;

	// 各種方向
	constexpr float DIRECTION_FORWARD = 0.f;
	constexpr float DIRECTION_RIGHT = -1.f;
	constexpr float DIRECTION_LEFT = 1.f;
	

	if (CurrentDirectionOfVelocity < VELOCITY_LEFT)
	{
		return DIRECTION_LEFT;
	}
	else if (CurrentDirectionOfVelocity > VELOCITY_RIGHT)
	{
		return DIRECTION_RIGHT;
	}
	else
	{
		return DIRECTION_FORWARD;
	}
}

#pragma optimize ("",on)

