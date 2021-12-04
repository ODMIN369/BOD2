// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CPP_TraceInteractiveTriggerBase.h"
#include "CPP_InteractDucumentItemBase.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FInteractDocumentItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText Text;

public:
	bool operator==(const FInteractDocumentItemData InItemData) { return this->Name == InItemData.Name; }

	void operator=(const FInteractDocumentItemData* InItemDataPtr)
	{
		if (InItemDataPtr == nullptr) { return; }
		this->Name = InItemDataPtr->Name;
		this->Text = InItemDataPtr->Text;
	}
};

/**
 * 
 */
UCLASS()
class O369_HORROR01_API ACPP_InteractDucumentItemBase : public ACPP_TraceInteractiveTriggerBase
{
	GENERATED_BODY()

public:
	ACPP_InteractDucumentItemBase();

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USoundBase* AcquisitionSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FName DocumentItem;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSoftObjectPtr<UDataTable> InteractDocumentItemDataTable;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FInteractDocumentItemData DocumentItemData;

public:
	virtual void OnTraceEvent_Implementation(FHitResult& HitResult) override;
};


