// Fill out your copyright notice in the Description page of Project Settings.


#include "PakHttp.h"
#include "HttpModule.h"
#include "Modules/ModuleManager.h"
#include "FileHelper.h"
#include "Paths.h"

FPakHttp::FPakHttp()
	: bDownloadSuccess(false)
{
}

FPakHttp::~FPakHttp()
{
}

void FPakHttp::DownLoad(FString& UrlIn)
{
	CancelDownload();

	// store the url
	Url = UrlIn;

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(Url);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/octet-stream"));
	HttpRequest->OnProcessRequestComplete().BindRaw(this, &FPakHttp::OnRequestComplete);
	HttpRequest->OnRequestProgress().BindRaw(this, &FPakHttp::OnRequestProgress);
	HttpRequest->ProcessRequest();
}

void FPakHttp::OnRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSent, int32 BytesReceived)
{
	int32 TotalLength = HttpRequest->GetContentLength();
	FString OutString = FString::FromInt(BytesReceived) / FString::FromInt(TotalLength);

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.0f, FColor::Red, FString::Printf(TEXT("BytesReceived: %s"), *OutString), false);
}

void FPakHttp::OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	if (!HttpRequest.IsValid() || !HttpResponse.IsValid())
	{
		return;
	}
	else if (bSucceeded && EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
	{
		FString Filename = FPaths::GetCleanFilename(HttpRequest->GetURL());
		bDownloadSuccess = FFileHelper::SaveArrayToFile(HttpResponse->GetContent(), \
			*FString::Printf(TEXT("%s%s"), *(FPaths::ProjectContentDir()+TEXT("DLC/")), *Filename));
	}
}

void FPakHttp::CancelDownload()
{
	bDownloadSuccess = false;
}

