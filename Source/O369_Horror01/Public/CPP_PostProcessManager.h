// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/PostProcessComponent.h"
#include "CPP_PostProcessManager.generated.h"

/**
 * 
 */
UCLASS()
class O369_HORROR01_API UCPP_PostProcessManager : public UObject
{
	GENERATED_BODY()

private:
	UPostProcessComponent* ApplyingColorGradingPostProcess = nullptr;

public:
	UFUNCTION(BlueprintPure)
		UPostProcessComponent* GetApplyingColorGradingPostProcess() { return ApplyingColorGradingPostProcess; }

	UFUNCTION(BlueprintCallable)
		void SetApplyingColorGradingPostProcess(UPostProcessComponent* InTargetPostProcess) { ApplyingColorGradingPostProcess = InTargetPostProcess; }

	UFUNCTION(BlueprintCallable)
		void DestroyApplyingColorGradingPostProcess()
	{
		if (ApplyingColorGradingPostProcess == nullptr) { return; }

		ApplyingColorGradingPostProcess->bEnabled = false;
		ApplyingColorGradingPostProcess->DestroyComponent();
	}
};
