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


	//GPTSOVITS_API ��һ���꣬����ָʾ�����������͵�����ţ�symbols���Ĺ淶
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
	
		//����String
		FGuid Request(
			const FString& InURL,
			const FString& InContent,
			const TMap<FString, FString>& InCustomMetadataHeader,
			bool bSynchronous = false,//�Ƿ�ͬ��
			GPTSoVITSHTTP::EHTTPVerbType VerbType = GPTSoVITSHTTP::EHTTPVerbType::GPTSOVITS_POST);

		//������
		FGuid Request(
			const FString& InURL,
			const TArray<uint8>& InByteData,
			const TMap<FString, FString>& InCustomMetadataHeader,
			bool bSynchronous = false,//�Ƿ�ͬ��
			GPTSoVITSHTTP::EHTTPVerbType VerbType = GPTSoVITSHTTP::EHTTPVerbType::GPTSOVITS_POST);
		
		//ȡ�������ж������Ƿ�������
		void CancelRequest(const FGuid& InRequestID);
		bool IsValidRequest(const FGuid& InRequestID) const;

		//request����
	private:
		FGuid Request(
			const FString& InURL,
			const FString& InContent,
			const TArray<uint8>& InByte,
			const TMap<FString, FString>& InCustomMetadataHeader,
			bool bSynchronous = false,//�Ƿ�ͬ��
			GPTSoVITSHTTP::EHTTPVerbType VerbType = GPTSoVITSHTTP::EHTTPVerbType::GPTSOVITS_POST);
	
	//�󶨻ص�
	private:
		void OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSsucceeded);
		void OnRequestProgress(FHttpRequestPtr HttpRequest, int32 SendBytes, int32 BytesReceived);
		void OnRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue);
	
	//POST��GET��ת�����ַ���
	private:
		FString HTTPVerbToString(GPTSoVITSHTTP::EHTTPVerbType InVerbType);

	//��Ա����
	private:
		TMap<FGuid, TSharedPtr<IHttpRequest, ESPMode::ThreadSafe>> HttpRequests;
		GPTSoVITSHTTP::FHTTPDelegate Delegate;
	};
}
