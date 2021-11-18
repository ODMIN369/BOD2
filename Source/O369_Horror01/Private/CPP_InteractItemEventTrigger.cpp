// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_InteractItemEventTrigger.h"
#include "CPP_InteractItemBase.h"
#include "CPP_O369GameInstance.h"
#include "Components/BoxComponent.h"


ACPP_InteractItemEventTrigger::ACPP_InteractItemEventTrigger()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACPP_InteractItemEventTrigger::OnTraceEvent_Implementation(FHitResult& HitResult)
{
	if (!UsePlayerInventoryItem()) 
	{
		OnFailedTraceDelegateEvent.Broadcast();
		OnFailedTraceEvent();
		UCPP_O369GameInstance::GetInstance()->GetAudioManager()->PlaySoundSe(this, FailedSound, 1.f, false, false, FVector::ZeroVector, 1.f, 0.f, 0.f, nullptr, nullptr, nullptr);

		return; 
	}

	TriggerCollision->SetCollisionProfileName("NoCollision");

	UCPP_O369GameInstance::GetInstance()->GetAudioManager()->PlaySoundSe(this, AcquisitionSound, 1.f, false,false, FVector::ZeroVector, 1.f, 0.f,0.f,nullptr,nullptr,nullptr);
	Super::OnTraceEvent_Implementation(HitResult);
}

bool ACPP_InteractItemEventTrigger::UsePlayerInventoryItem()
{
	constexpr int32 NOT_REQUIRED_ITEM = 0;
	if (RequiredItemIDs.Num() <= NOT_REQUIRED_ITEM) { return false; }

	TArray<FInteractItemData>* PlayerInteractItems = UCPP_O369GameInstance::GetInstance()->GetPlayerInventoryBase()->GetInteractItems();

	TArray<FInteractItemData> UseTargetItemsData;
	UseTargetItemsData.Reserve(RequiredItemIDs.Num());

	int32 FoundCount = 0;
	for (const FString& RequiredItemID : RequiredItemIDs)
	{
		for(int32 i = 0 ; i < PlayerInteractItems->Num(); i++)
		{
			FInteractItemData PlayerInteractItem = PlayerInteractItems->GetData()[i];
			if (PlayerInteractItem.ID == RequiredItemID) 
			{
				UseTargetItemsData.Add(PlayerInteractItem);
				FoundCount++;
				break; 
			}
		}
	}

	// 対象のアイテムを全て見つけられなかった
	if (FoundCount < RequiredItemIDs.Num()) { return false; }

	for (const FInteractItemData& UseItemDataTarget : UseTargetItemsData)
	{
		PlayerInteractItems->RemoveSwap(UseItemDataTarget);
	}

	return true;
}