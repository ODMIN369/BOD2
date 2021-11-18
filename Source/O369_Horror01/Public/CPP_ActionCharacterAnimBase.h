// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_ParkourCharacterAnimBase.h"
#include "CPP_ActionCharacterAnimInterface.h"
#include "CPP_ActionCharacterAnimBase.generated.h"

/**
 * 
 */
UCLASS()
class O369_HORROR01_API UCPP_ActionCharacterAnimBase : public UCPP_ParkourCharacterAnimBase, public ICPP_ActionCharacterAnimInterface
{
	GENERATED_BODY()
	
private:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly)
		bool bIsAds;

public:
	void SetAds(bool InIsAds);
	void SetAds_Implementation(bool InIsAds) override;
};
