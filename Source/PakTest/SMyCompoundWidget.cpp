// Fill out your copyright notice in the Description page of Project Settings.


#include "SMyCompoundWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "SBoxPanel.h"
#include "SBox.h"
#include "FileManagerGeneric.h"
#include "Internationalization.h"

#define LOCTEXT_NAMESPACE "PakTestUI"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMyCompoundWidget::Construct(const FArguments& InArgs)
{
	TArray<TSharedPtr<sFileLayer>> _FileList = GetAllLocalPakFile();

	ChildSlot
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			.AutoWidth()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()				
				[
					SNew(SBox)
					.WidthOverride(200)
					[
						SAssignNew(PakScrollPtr,SScrollBox)
					]
				]
			]
		];

	for (int32 Index = 0; Index < _FileList.Num(); Index++)
	{
		PakScrollPtr->AddSlot()
			[
				BuildItem(_FileList[Index])
			];
	}
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedRef<SWidget> SMyCompoundWidget::BuildItem(TSharedPtr<sFileLayer>& FileInfo)
{
	TSharedPtr<SVerticalBox> VerPtr;

	SAssignNew(VerPtr,SVerticalBox)
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(this, &SMyCompoundWidget::GetPakType, FileInfo->Type)
				.ColorAndOpacity(FLinearColor::Blue)
			]
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(this, &SMyCompoundWidget::GetPakName, FileInfo->Name)
				.ColorAndOpacity(FLinearColor::White)
			]
			+ SHorizontalBox::Slot()
			[
				SNew(SButton)
			]
		];

	return VerPtr.ToSharedRef();
}

FText SMyCompoundWidget::GetPakType(FString PakName) const
{
	return LOCTEXT("Show Console", "Show Console");
}

FText SMyCompoundWidget::GetPakName(FString PakName) const
{
	return FText();
}

FReply SMyCompoundWidget::OutPut()
{
	return FReply::Handled();
}

TArray<TSharedPtr<sFileLayer>> SMyCompoundWidget::GetAllLocalPakFile()
{
	TArray<TSharedPtr<sFileLayer>> FileLayerList;
	TArray<FString> AbsolutePath, FileNames;
	IFileManager::Get().FindFilesRecursive(AbsolutePath, *(FPaths::ProjectContentDir() + TEXT("DLC/")), TEXT("*.pak"), true, false);

	for (auto FilePath : AbsolutePath)
	{
		FString FileName = FPaths::GetCleanFilename(FilePath);
		FileNames.Add(FileName);
	}

	return FileLayerList;
}