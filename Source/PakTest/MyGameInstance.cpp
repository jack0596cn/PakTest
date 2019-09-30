// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "PlatformFilemanager.h"
#include "IPlatformFilePak.h"
#include "Engine/StreamableManager.h"
#include "PackageName.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerController.h"
#include "PakTestCharacter.h"
#include "AssetRegistryModule.h"
#include "JsonSerializerMacros.h"
#include "ArrayReader.h"
#include "FileHelper.h"

void UMyGameInstance::Init()
{
	bool bSuccess = FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*(FPaths::ProjectContentDir() + TEXT("PakFiles")));
	if (bSuccess)
	{
		
	}
}

void UMyGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
}

void UMyGameInstance::MountMapPak(FString& MapName)
{
	FString SaveContentDir = FPaths::ProjectContentDir() + MapName;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	//初始化PakPlatformFile  
	FPakPlatformFile* PakPlatformFile = new FPakPlatformFile();
	PakPlatformFile->Initialize(&PlatformFile, TEXT(""));
	FPlatformFileManager::Get().SetPlatformFile(*PakPlatformFile);

	//获取Pak文件
	FPakFile PakFile(&PlatformFile, *SaveContentDir, false);
	//设置Pak文件的Mount点
	FString MountPoint(FPaths::ProjectContentDir()); 
	PakFile.SetMountPoint(*MountPoint);
	//对Pak文件Mount到前面定义的MountPoint
	if (PakPlatformFile->Mount(*SaveContentDir, 0, *MountPoint))
	{
		TArray<FString> FileList;
		//得到Pak文件中MountPoint路径下的所有文件
		PakFile.FindFilesAtPath(FileList, *PakFile.GetMountPoint(), true, false, true);
		FStreamableManager StreamableManager;
		//对文件的路径进行处理，转换成StaticLoadObject的路径格式
		for (int32 Index=0; Index < FileList.Num(); Index++)
		{
			FString AssetName = FileList[Index];
			FString AssetShortName = FPackageName::GetShortName(AssetName);
			FString LeftStr;
			FString RightStr;
			AssetShortName.Split(TEXT("."), &LeftStr, &RightStr);
			AssetName = TEXT("/Game/") + LeftStr + TEXT(".") + LeftStr;
			FStringAssetReference reference = AssetName;
			//Load UObject  
			UObject* LoadObject = StreamableManager.LoadSynchronous(reference);
			if (LoadObject != nullptr)
			{
				UE_LOG(LogTemp, Log, TEXT("Object Load Success..."))
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Can not Load asset..."))
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Mount Failed"));
	}
}