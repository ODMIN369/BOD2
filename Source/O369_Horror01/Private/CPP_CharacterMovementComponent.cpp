// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_CharacterMovementComponent.h"

#include "CPP_TransformComponent.h"
#include "CPP_CharacterBase.h"

#include "Components/MeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "GameFramework/Character.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

UCPP_CharacterMovementComponent::UCPP_CharacterMovementComponent()
{
	CurrentGroundSurfaceType = EPhysicalSurface::SurfaceType_Default;

	SetComponentTickEnabled(true);
}

void UCPP_CharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OriginCharacterOwner = Cast<ACPP_CharacterBase>(GetCharacterOwner());

	GroundCheckQueryParams.AddIgnoredActor(GetCharacterOwner());
	GroundCheckQueryParams.bReturnPhysicalMaterial = true;
}

void UCPP_CharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckGroundSurfaceType(CurrentGroundSurfaceType);

	if (IsCrouching())
	{
		MaxWalkSpeedCrouched = CrouchMoveSpeed;
	}
}

bool UCPP_CharacterMovementComponent::CheckGroundSurfaceType(EPhysicalSurface& OutSurfaceType)
{
	const FVector HeadLocation = OriginCharacterOwner->GetHeadRootComponent()->GetComponentLocation();
	const FVector FootLocation = OriginCharacterOwner->GetFootRootComponent()->GetComponentLocation();
	constexpr float TRACE_GROUND = 50.f;
	const FVector EndGroundTraceLocation = FVector(FootLocation.X, FootLocation.Y, FootLocation.Z - TRACE_GROUND);

	constexpr float DEBUGLINE_VISIBLE_TIME = 3.f;
	constexpr float DEBUGLINE_THICKNESS = 2.f;
	constexpr uint8 DEBUGLINE_DEPTH = 0;

	//DrawDebugLine(GetWorld(), HeadLocation, EndGroundTraceLocation, FColor::Red, false, DEBUGLINE_VISIBLE_TIME, DEBUGLINE_DEPTH, DEBUGLINE_THICKNESS);

	FHitResult GroundHitResult;
	if (GetWorld()->LineTraceSingleByChannel(GroundHitResult, HeadLocation, EndGroundTraceLocation, ECollisionChannel::ECC_Visibility, GroundCheckQueryParams))
	{
		EPhysicalSurface SurfaceType = UGameplayStatics::GetSurfaceType(GroundHitResult);
		if (SurfaceType == EPhysicalSurface::SurfaceType_Default) 
		{
			return false; 
		}

		CurrentHitSurfaceTracePoint = GroundHitResult.ImpactPoint;

		if (SurfaceType == CurrentGroundSurfaceType) { return false; }

		OutSurfaceType = SurfaceType;

		return true;
	}

	return false;
}

void UCPP_CharacterMovementComponent::SetGroundSurfaceTypeForHitResult(const FHitResult& InHitResult)
{
	// メッシュから取得
	EPhysicalSurface SurfaceType = UGameplayStatics::GetSurfaceType(InHitResult);

	// メッシュにPhysicalMaterialがない場合
	if (SurfaceType == EPhysicalSurface::SurfaceType_Default) 
	{
		if (InHitResult.Component == nullptr) { return; }
		const UMeshComponent* HitResultActorMesh = Cast<UMeshComponent>(InHitResult.Component);

		if (HitResultActorMesh == nullptr) { return; }

		// マテリアルから取得
		UMaterialInterface* HitResultMaterial = HitResultActorMesh->GetMaterial(0);
		if (HitResultMaterial == nullptr) { return; }

		UPhysicalMaterial* HitResultPhysicalMaterial = HitResultMaterial->GetPhysicalMaterial();
		if (HitResultPhysicalMaterial == nullptr) { return; }

		EPhysicalSurface HitCompPhysicalSurface = HitResultPhysicalMaterial->SurfaceType;

		if (HitCompPhysicalSurface == EPhysicalSurface::SurfaceType_Default) { return; }

		SurfaceType = HitCompPhysicalSurface;
	}
	if (SurfaceType == CurrentGroundSurfaceType) { return; }

	SetGroundSurfaceType(SurfaceType);
}
