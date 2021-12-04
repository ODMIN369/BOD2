// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_InteractDucumentItemBase.h"
#include "CPP_O369GameInstance.h"
#include "Components/BoxComponent.h"

ACPP_InteractDucumentItemBase::ACPP_InteractDucumentItemBase()
{
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(Root);
	ItemMesh->SetMobility(EComponentMobility::Static);
	ItemMesh->SetGenerateOverlapEvents(false);
	ItemMesh->SetCollisionProfileName(TEXT("BlockAll"));
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	ItemMesh->SetComponentTickEnabled(false);

	InteractDocumentItemDataTable = TSoftObjectPtr<UDataTable>(FSoftObjectPath(TEXT("/Game/Kobayashi/DataTable/DT_InteractDocumentItem.DT_InteractDocumentItem"))).LoadSynchronous();

	PrimaryActorTick.bCanEverTick = false;
}

void ACPP_InteractDucumentItemBase::BeginPlay()
{
	Super::BeginPlay();
	if (InteractDocumentItemDataTable.IsValid())
	{
		FString ContextString;
		const FInteractDocumentItemData* InteractItemDocumentDataRow = InteractDocumentItemDataTable.Get()->FindRow<FInteractDocumentItemData>(DocumentItem, ContextString);
		DocumentItemData = InteractItemDocumentDataRow;
	}
}

void ACPP_InteractDucumentItemBase::OnTraceEvent_Implementation(FHitResult& HitResult)
{
	UCPP_O369GameInstance* const GameInstance = UCPP_O369GameInstance::GetInstance();

	GameInstance->GetPlayerInventoryBase()->GetInteractDocumentItems()->Add(DocumentItemData);
	TriggerCollision->SetCollisionProfileName(TEXT("NoCollision"));
	GameInstance->GetAudioManager()->PlaySoundSe(this, AcquisitionSound, 1.f, false, false, FVector::ZeroVector, 1.f, 0.f, 0.f, nullptr, nullptr, nullptr);
	Super::OnTraceEvent_Implementation(HitResult);

	SetLifeSpan(KINDA_SMALL_NUMBER);

}