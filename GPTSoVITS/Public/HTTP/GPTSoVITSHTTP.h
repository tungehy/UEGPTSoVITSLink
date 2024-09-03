#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "GPTSoVITSType.h"

namespace GPTSoVITSHTTP
{
	enum class EHTTPVerbType :uint8
	{
		GPTSOVITS_POST,
		GPTSOVITS_PUT,
		GPTSOVITS_GET,
	};


	//GPTSOVITS_API 是一个宏，用来指示编译器导出和导入符号（symbols）的规范
	struct GPTSOVITS_API FHTTPDelegate
	{
		FHttpRequestCompleteDelegate					SimpleCompleteDelegate;
		FHttpRequestProgressDelegate                    SimpleSingleRequestProgressDelegate;
		FHttpRequestHeaderReceivedDelegate              SimpleSingleRequestHeaderReceivedDelegate;
	};

	struct GPTSOVITS_API FHTTP :public TSharedFromThis<FHTTP>
	{

		static TSharedPtr<FHTTP> CreateHTTPObject(FHTTPDelegate InDelegate);
	
	public:
		FHTTP(FHTTPDelegate InDelegate);
	
		//请求String
		FGuid Request(
			const FString& InURL,
			const FString& InContent,
			const TMap<FString, FString>& InCustomMetadataHeader,
			bool bSynchronous = false,//是否同步
			GPTSoVITSHTTP::EHTTPVerbType VerbType = GPTSoVITSHTTP::EHTTPVerbType::GPTSOVITS_POST);

		//二进制
		FGuid Request(
			const FString& InURL,
			const TArray<uint8>& InByteData,
			const TMap<FString, FString>& InCustomMetadataHeader,
			bool bSynchronous = false,//是否同步
			GPTSoVITSHTTP::EHTTPVerbType VerbType = GPTSoVITSHTTP::EHTTPVerbType::GPTSOVITS_POST);
		
		//取消请求，判断请求是否有意义
		void CancelRequest(const FGuid& InRequestID);
		bool IsValidRequest(const FGuid& InRequestID) const;

		//request整合
	private:
		FGuid Request(
			const FString& InURL,
			const FString& InContent,
			const TArray<uint8>& InByte,
			const TMap<FString, FString>& InCustomMetadataHeader,
			bool bSynchronous = false,//是否同步
			GPTSoVITSHTTP::EHTTPVerbType VerbType = GPTSoVITSHTTP::EHTTPVerbType::GPTSOVITS_POST);
	
	//绑定回调
	private:
		void OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSsucceeded);
		void OnRequestProgress(FHttpRequestPtr HttpRequest, int32 SendBytes, int32 BytesReceived);
		void OnRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue);
	
	//POST、GET等转换成字符串
	private:
		FString HTTPVerbToString(GPTSoVITSHTTP::EHTTPVerbType InVerbType);

	//成员变量
	private:
		TMap<FGuid, TSharedPtr<IHttpRequest, ESPMode::ThreadSafe>> HttpRequests;
		GPTSoVITSHTTP::FHTTPDelegate Delegate;
	};
}
