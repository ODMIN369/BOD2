// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CPP_TransformComponent.generated.h"


UCLASS(BlueprintType)
class O369_HORROR01_API UCPP_TransformComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	void Initialize(USceneComponent* InParent = nullptr);
};
