// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PAKTEST_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init();
	virtual void LoadComplete(const float LoadTime, const FString& MapName);

	void MountMapPak(FString& MapName);

	bool LoadPakComponent(FString& AssetName);
};
