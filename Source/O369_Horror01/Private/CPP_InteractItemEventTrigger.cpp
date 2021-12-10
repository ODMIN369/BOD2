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
	//使用するアイテムがない場合は強制的に失敗
	if (RequiredItemIds.Num() <= NOT_REQUIRED_ITEM) { return false; }

	// インベントリにあるアイテム
	TArray<FInteractItemData>* PlayerInteractItems = UCPP_O369GameInstance::GetInstance()->GetPlayerInventoryBase()->GetInteractItems();

	// これから使用されるアイテムのキャッシュ
	TArray<FInteractItemData> UseTargetItemsData;
	UseTargetItemsData.Reserve(RequiredItemIds.Num());

	int32 FoundCount = 0;
	// 必要なアイテム分インベントリから探す
	for (const FName& RequiredItemId : RequiredItemIds)
	{
		for(int32 i = 0 ; i < PlayerInteractItems->Num(); i++)
		{
			FInteractItemData PlayerInteractItem = PlayerInteractItems->GetData()[i];
			if (PlayerInteractItem.Id == RequiredItemId)
			{
				// 使用するアイテムを確定させる
				UseTargetItemsData.Add(PlayerInteractItem);
				FoundCount++;
				break; 
			}
		}
	}

	// 使用対象のアイテムを全て見つけられなかった
	if (FoundCount < RequiredItemIds.Num()) { return false; }

	for (const FInteractItemData& UseItemDataTarget : UseTargetItemsData)
	{
		// 使用したアイテムをインベントリから削除する
		PlayerInteractItems->RemoveSwap(UseItemDataTarget);
	}

	return true;
}