// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CPP_InteractItemBase.h"
#include "CPP_PlayerInventoryBase.generated.h"

/**
 * 
 */
UCLASS()
class O369_HORROR01_API UCPP_PlayerInventoryBase : public UObject
{
	GENERATED_BODY()

public:
	UCPP_PlayerInventoryBase();

protected:
	UPROPERTY()
	TArray<FInteractItemData> InteractItems;

public:
	TArray<FInteractItemData>* GetInteractItems() { return &InteractItems; }
};
