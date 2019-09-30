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
	//��ʼ��PakPlatformFile  
	FPakPlatformFile* PakPlatformFile = new FPakPlatformFile();
	PakPlatformFile->Initialize(&PlatformFile, TEXT(""));
	FPlatformFileManager::Get().SetPlatformFile(*PakPlatformFile);

	//��ȡPak�ļ�
	FPakFile PakFile(&PlatformFile, *SaveContentDir, false);
	//����Pak�ļ���Mount��
	FString MountPoint(FPaths::ProjectContentDir()); 
	PakFile.SetMountPoint(*MountPoint);
	//��Pak�ļ�Mount��ǰ�涨���MountPoint
	if (PakPlatformFile->Mount(*SaveContentDir, 0, *MountPoint))
	{
		TArray<FString> FileList;
		//�õ�Pak�ļ���MountPoint·���µ������ļ�
		PakFile.FindFilesAtPath(FileList, *PakFile.GetMountPoint(), true, false, true);
		FStreamableManager StreamableManager;
		//���ļ���·�����д���ת����StaticLoadObject��·����ʽ
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