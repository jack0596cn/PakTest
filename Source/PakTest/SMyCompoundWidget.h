// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SScrollBox.h"

struct sFileLayer
{
	sFileLayer(FString _Type, FString _Name, FString _Url):
		Type(_Type),Name(_Name),Url(_Url)
	{
	}
	~sFileLayer()
	{
	}

	FString Type;
	FString Name;
	FString Url;
};

/**
 * 
 */
class PAKTEST_API SMyCompoundWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMyCompoundWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FReply OutPut();

	TSharedRef<SWidget> BuildItem(TSharedPtr<sFileLayer>& FileInfo);

	FText GetPakType(FString PakName) const;
	FText GetPakName(FString PakName) const;
	
	TArray<TSharedPtr<sFileLayer>> GetAllLocalPakFile();

private:
	TSharedPtr<SScrollBox> PakScrollPtr;
	TArray<TSharedPtr<sFileLayer>> FileList;

};
