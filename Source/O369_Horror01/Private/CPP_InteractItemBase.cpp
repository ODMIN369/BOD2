// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_InteractItemBase.h"
#include "CPP_O369GameInstance.h"
#include "Components/BoxComponent.h"

ACPP_InteractItemBase::ACPP_InteractItemBase()
{
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(Root);
	ItemMesh->SetMobility(EComponentMobility::Static);
	ItemMesh->SetGenerateOverlapEvents(false);
	ItemMesh->SetCollisionProfileName(TEXT("BlockAll"));
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	ItemMesh->SetComponentTickEnabled(false);

	InteractItemDataTable = TSoftObjectPtr<UDataTable>(FSoftObjectPath(TEXT("/Game/O369Studio/DataTable/DT_InteractItem.DT_InteractItem"))).LoadSynchronous();

	PrimaryActorTick.bCanEverTick = false;
}

void ACPP_InteractItemBase::BeginPlay()
{
	Super::BeginPlay();
	if (InteractItemDataTable.IsValid())
	{
		FString ContextString;
		const FInteractItemData* InteractItemDataRow = InteractItemDataTable.Get()->FindRow<FInteractItemData>(ItemName, ContextString);
		ItemData = InteractItemDataRow;
	}
}

void ACPP_InteractItemBase::OnTraceEvent_Implementation(FHitResult& HitResult)
{
	UCPP_O369GameInstance* const GameInstance = UCPP_O369GameInstance::GetInstance();

	GameInstance->GetPlayerInventoryBase()->GetInteractItems()->Add(ItemData);
	TriggerCollision->SetCollisionProfileName(TEXT("NoCollision"));
	GameInstance->GetAudioManager()->PlaySoundSe(this,AcquisitionSound, 1.f, false,false, FVector::ZeroVector, 1.f, 0.f,0.f, nullptr, nullptr, nullptr);
	Super::OnTraceEvent_Implementation(HitResult);

	SetLifeSpan(KINDA_SMALL_NUMBER);
	
}