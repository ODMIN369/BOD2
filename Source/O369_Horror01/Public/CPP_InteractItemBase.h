// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CPP_TraceInteractiveTriggerBase.h"
#include "CPP_InteractItemBase.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FInteractItemData : public FTableRowBase
{
	GENERATED_BODY()
		FName Id;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText Text;

public:
	bool operator==(const FInteractItemData InItemData) { return this->Id == InItemData.Id; }

	void operator=(const FInteractItemData* InItemDataPtr)
	{
		if (InItemDataPtr == nullptr) { return; }
		this->Name = InItemDataPtr->Name;
		this->Text = InItemDataPtr->Text;
	}
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class O369_HORROR01_API ACPP_InteractItemBase : public ACPP_TraceInteractiveTriggerBase
{
	GENERATED_BODY()
	
public:
	ACPP_InteractItemBase();

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USoundBase* AcquisitionSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FName ItemName;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly , meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UDataTable> InteractItemDataTable;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FInteractItemData ItemData;

public:
	virtual void OnTraceEvent_Implementation(FHitResult& HitResult) override;

};
