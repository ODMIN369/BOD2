// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_TransformComponent.h"

void UCPP_TransformComponent::Initialize(USceneComponent* InParent)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetComponentTickEnabled(false);
	SetActive(false);
	SetVisibility(false);

	if (InParent == nullptr) { return; }
	SetupAttachment(InParent);
}