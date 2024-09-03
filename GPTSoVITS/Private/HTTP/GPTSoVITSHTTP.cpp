#include "HTTP/GPTSoVITSHTTP.h"
#include "HttpManager.h"

namespace GPTSoVITSHTTP
{

	TSharedPtr<FHTTP> FHTTP::CreateHTTPObject(FHTTPDelegate InDelegate)
	{
		return MakeShareable(new GPTSoVITSHTTP::FHTTP(InDelegate));
	}

	FHTTP::FHTTP(FHTTPDelegate InDelegate)
		:Delegate(InDelegate)
	{

	}

	FGuid FHTTP::Request(
		const FString& InURL,
		const FString& InContent,
		const TMap<FString, FString>& InCustomMetadataHeader,
		bool bSynchronous,//是否同步
		GPTSoVITSHTTP::EHTTPVerbType VerbType)
	{
		return Request(InURL, InContent, TArray<uint8>(), InCustomMetadataHeader, bSynchronous, VerbType);
	}

	FGuid FHTTP::Request(
		const FString& InURL,
		const TArray<uint8>& InByteData,
		const TMap<FString, FString>& InCustomMetadataHeader,
		bool bSynchronous,//是否同步
		GPTSoVITSHTTP::EHTTPVerbType VerbType)
	{
		return Request(InURL, TEXT(""), InByteData, InCustomMetadataHeader, bSynchronous, VerbType);
	}

	FGuid FHTTP::Request(
		const FString& InURL,
		const FString& InContent,
		const TArray<uint8>& InByte,
		const TMap<FString, FString>& InCustomMetadataHeader,
		bool bSynchronous,//是否同步
		GPTSoVITSHTTP::EHTTPVerbType VerbType)
	{
		FGuid RequestID = FGuid::NewGuid();
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(InURL);

		Request->SetTimeout(15.f);

		Request->SetHeader(TEXT("RequestID"), RequestID.ToString());

		for (auto& Tmp : InCustomMetadataHeader)
		{
			Request->SetHeader(Tmp.Key, Tmp.Value);
		}

		Request->SetVerb(GPTSoVITSHTTP::FHTTP::HTTPVerbToString(VerbType));

		if (InByte.Num() > 0)
		{
			Request->SetContent(InByte);
		}

		else if (!InContent.IsEmpty())
		{
			Request->SetContentAsString(InContent);
		}

		Request->OnProcessRequestComplete().BindSP(this, &FHTTP::OnRequestComplete);
		Request->OnRequestProgress().BindSP(this, &FHTTP::OnRequestProgress);
		Request->OnHeaderReceived().BindSP(this, &FHTTP::OnRequestHeaderReceived);

		bool bRequest = Request->ProcessRequest();
		if (bRequest)
		{
			HttpRequests.Add(RequestID, Request);
			if (bSynchronous)
			{
				FHttpModule::Get().GetHttpManager().Flush(EHttpFlushReason::Default);
			}
		}
		else
		{
			RequestID = FGuid();
		}

		return RequestID;
	}

	void FHTTP::OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
	{
		FString RequestIDString = HttpRequest->GetHeader(TEXT("RequestID"));
		FGuid RequestID;
		FGuid::Parse(RequestIDString, RequestID);

		HttpRequests.Remove(RequestID);

		Delegate.SimpleCompleteDelegate.ExecuteIfBound(HttpRequest, HttpResponse, bSucceeded);

	}

	void FHTTP::OnRequestProgress(FHttpRequestPtr HttpRequest, int32 SendBytes, int32 BytesReceived)
	{
		Delegate.SimpleSingleRequestProgressDelegate.ExecuteIfBound(HttpRequest, SendBytes, BytesReceived);
	}

	void FHTTP::OnRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
	{
		Delegate.SimpleSingleRequestHeaderReceivedDelegate.ExecuteIfBound(Request, HeaderName, NewHeaderValue);
	}

	FString FHTTP::HTTPVerbToString(GPTSoVITSHTTP::EHTTPVerbType InVerbType)
	{
		switch (InVerbType)
		{
		case GPTSoVITSHTTP::EHTTPVerbType::GPTSOVITS_POST:
			return TEXT("POST");
		case GPTSoVITSHTTP::EHTTPVerbType::GPTSOVITS_PUT:
			return TEXT("PUT");
		case GPTSoVITSHTTP::EHTTPVerbType::GPTSOVITS_GET:
			return TEXT("GET");

		}
		return TEXT("POST");
	}

	void FHTTP::CancelRequest(const FGuid& InRequestID)
	{
		if (TSharedPtr<IHttpRequest, ESPMode::ThreadSafe>* HttpRequest = HttpRequests.Find(InRequestID))
		{
			(*HttpRequest)->CancelRequest();
		}
	}

	bool FHTTP::IsValidRequest(const FGuid& InRequestID) const
	{
		return HttpRequests.Contains(InRequestID);
	}

}