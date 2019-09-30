// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Templates/SharedPointer.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

typedef TSharedPtr<class IHttpRequest> FHttpRequestPtr;
typedef TSharedPtr<class IHttpResponse, ESPMode::ThreadSafe> FHttpResponsePtr;

/**
 * 
 */
class PAKTEST_API FPakHttp :public TSharedFromThis<FPakHttp>
{
public:
	FPakHttp();
	virtual ~FPakHttp();

public:
	void OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	void OnRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSent, int32 BytesReceived);

	/** Cancel any download in progress */
	void CancelDownload();
	void DownLoad(FString& UrlIn);

private:
	/** The Url being downloaded */
	FString Url;
	/** Have we successfully downloaded the URL we asked for */
	bool bDownloadSuccess;
};
