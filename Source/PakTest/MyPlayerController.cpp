// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "CoreDelegates.h"
#include "Paths.h"
#include "PlatformFilemanager.h"
#include "IPlatformFilePak.h"
#include "Engine/StreamableManager.h"
#include "PackageName.h"
#include "PakTestCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerController.h"
#include "PakTestCharacter.h"
#include "PakHttp.h"

#define LOCTEXT_NAMESPACE "PakTestUI"

AMyPlayerController::AMyPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AC = nullptr;
}

void AMyPlayerController::BeginPlay()
{
	//FString ComName("ComTest.pak");
	//MountComponent(ComName);


	TSharedPtr<FPakHttp> PakDownloader = MakeShareable(new FPakHttp());
	if (PakDownloader.IsValid())
	{
		FString Url = FString("http://10.11.34.116/PakFiles/ComTest.pak");
		PakDownloader->DownLoad(Url);
	}

	Super::BeginPlay();
}

void AMyPlayerController::MountComponent(FString& ComName)
{
	FString SaveContentDir = FPaths::ProjectContentDir() + ComName;

	IPlatformFile&PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	FPakPlatformFile*PakPlatformFile = new FPakPlatformFile();

	PakPlatformFile->Initialize(&PlatformFile, TEXT(""));

	FPlatformFileManager::Get().SetPlatformFile(*PakPlatformFile);

	FPakFile PakFile(&PlatformFile, *SaveContentDir, false);

	FString MountPoint(FPaths::ProjectContentDir());

	PakFile.SetMountPoint(*MountPoint);

	if (PakPlatformFile->Mount(*SaveContentDir, 0, *MountPoint))
	{
		TArray<FString> FileList;

		PakFile.FindFilesAtPath(FileList, *PakFile.GetMountPoint(), true, false, true);

		FString AssetName = FileList[0];
		FString AssetShortName = FPackageName::GetShortName(AssetName);
		FString LeftStr, RightStr;
		AssetShortName.Split(TEXT("."), &LeftStr, &RightStr);
		AssetName = TEXT("/Game/") + LeftStr + TEXT(".") + LeftStr;

		TSubclassOf<UActorComponent> ActorComClass = LoadClass<UActorComponent>(nullptr, *(AssetName + TEXT("_C")), nullptr);

		APakTestCharacter* Cha = Cast<APakTestCharacter>(GetCharacter());

		if (ActorComClass != nullptr)
		{
			UActorComponent* ActorCom = NewObject<UActorComponent>(Cha, ActorComClass);
			if (ActorCom)
			{
				Cha->AddOwnedComponent(ActorCom);
				ActorCom->RegisterComponent();
			}
		}
	}
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.0f, FColor::Green, TEXT("中文字符"), false);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.0f, FColor::Red, NSLOCTEXT("PakTestUI", "Show Console Test", "Show Console Test").ToString(), false);

	Super::PlayerTick(DeltaTime);
}

#undef LOCTEXT_NAMESPACE

