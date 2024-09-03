#include "GPTSoVITSManage.h"
#include "GPTSoVITSLog.h"
#include "Json/GPTSoVITSJson.h"

FGPTSoVITSManage* FGPTSoVITSManage::Instance = nullptr;

//声明绑定代理
FGPTSoVITSManage::FGPTSoVITSManage()
{
	GPTSoVITSHTTP::FHTTPDelegate HTTPDelegate;
	HTTPDelegate.SimpleCompleteDelegate.BindLambda(
		[&](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
		{
			OnRequestComplete(HttpRequest, HttpResponse, bSucceeded);
		});
	HTTPDelegate.SimpleSingleRequestProgressDelegate.BindLambda(
		[&](FHttpRequestPtr HttpRequest, int32 SendBytes, int32 BytesReceived)
		{
			OnRequestProgress(HttpRequest, SendBytes, BytesReceived);
		});
	HTTPDelegate.SimpleSingleRequestHeaderReceivedDelegate.BindLambda(
		[&](FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
		{
			OnRequestHeaderReceived(Request, HeaderName, NewHeaderValue);
		});

	HTTP = GPTSoVITSHTTP::FHTTP::CreateHTTPObject(HTTPDelegate);

	WebSocket = MakeShareable(new FGPTSoVITSWebSocket);

	WebSocket->OnClosedDelegate.BindRaw(this, &FGPTSoVITSManage::OnClosed); 
	WebSocket->OnConnectedDelegate.BindRaw(this, &FGPTSoVITSManage::OnConnected);
	WebSocket->OnConnectionErrorDelegate.BindRaw(this, &FGPTSoVITSManage::OnConnectionError);
	WebSocket->OnMessageSentDelegate.BindRaw(this, &FGPTSoVITSManage::OnMessagesent);
	WebSocket->OnMessageDelegate.BindRaw(this, &FGPTSoVITSManage::OnMessage);
	WebSocket->OnRawMessageDelegate.BindRaw(this, &FGPTSoVITSManage::OnRawMessage);


	URL = TEXT("http://127.0.0.1:9880");

	TTSWebSocketRecognitionSynthesisStreaming = MakeShareable(new FTTSWebSocketRecognitionSynthesis); 
	TTSWebSocketSpeechSynthesisStreaming = MakeShareable(new FTTSWebSocketSpeechSynthesis);

}

// 获取单例实例
FGPTSoVITSManage* FGPTSoVITSManage::Get()
{
	if (!Instance)
	{
		Instance = new FGPTSoVITSManage();
		Instance->Init();
	}
	
	return Instance;
}

void FGPTSoVITSManage::Destory()
{
	if (Instance)
	{
		delete Instance;
	}

	Instance = NULL;
}

void FGPTSoVITSManage::Init()
{
}

FGuid FGPTSoVITSManage::SpeechSynthesis(const FString& InRequest)
{
	if (HTTP)
	{
		FString MyURL = FString::Printf(TEXT("%s/tts"),*URL);

		TMap<FString,FString> Meta;
		Meta.Add(TEXT("Content-Type"), TEXT("application/json"));
		
		Meta.Add(TEXT("Protocol"), TEXT("SpeechSynthesis"));
		return HTTP->Request(MyURL, InRequest, Meta);
	}
	return FGuid();
}

FGuid FGPTSoVITSManage::SpeechSynthesis(const FGPTSoVITSSynthesisRequest& InRequest)
{
	FString JsonString;
	GPTSoVITSJson::SpeechSynthesisRequestToJsonString(InRequest, JsonString);

	return SpeechSynthesis(JsonString);
}

void FGPTSoVITSManage::CancelRequest(const FGuid& InRequestID)
{
	HTTP->CancelRequest(InRequestID);
}

void FGPTSoVITSManage::SetURL(const FString& InURL)
{
	URL = InURL;

	FString DomainString; 
	GetDomain(DomainString);

	//FString wsRecognitionASRuRL = FString::Printf(TEXT("ws://%s/asr"), *DomainString);
	//TTSWebSocketRecognitionSynthesisStreaming->Init(WSRecognitionTTSURL);
	FString wssynthesisAsRURL = FString::Printf(TEXT("ws://%s/tts"), *DomainString);
	TTSWebSocketSpeechSynthesisStreaming->Init(WSSynthesisTTSURL);

}

bool FGPTSoVITSManage::IsValidRequest(const FGuid& InRequestID) const
{
	return HTTP->IsValidRequest(InRequestID);
}

void FGPTSoVITSManage::Link()
{
	WebSocket->Link();
}

void FGPTSoVITSManage::Relink()
{
	WebSocket->Relink();
}

void FGPTSoVITSManage::Unlink()
{
	WebSocket->Unlink();
}

bool FGPTSoVITSManage::IsConnected()
{
	return WebSocket->IsConnected();;
}

void FGPTSoVITSManage::DebugCode(int32 InCode)
{
	switch (InCode)
	{
	case 200://服务正常
		UE_LOG(GPTSoVITSLog, Log, TEXT("[200] success."));
		break;
	case 400://请求参数设置不正常
		UE_LOG(GPTSoVITSLog, Error, TEXT("[400] Request parameter setting incorrect."));
		break;
	case 404://网址不正常
		UE_LOG(GPTSoVITSLog, Error, TEXT("[404] Incorrect website address."));
		break;
	case 500://服务器内部错误
		UE_LOG(GPTSoVITSLog, Warning, TEXT("[500] Server Internal Error."));
		break;
	case 502://服务器内部io错误
		UE_LOG(GPTSoVITSLog, Warning, TEXT("[502] Server Internal network IO exception."));
		break;
	case 509://未知错误
		UE_LOG(GPTSoVITSLog, Warning, TEXT("[509] unknow error."));
		break;
	default :
		UE_LOG(GPTSoVITSLog, Error, TEXT("[%i] unable to identify error types."), InCode);
		break;
	}
}

//提取域名
void FGPTSoVITSManage::GetDomain(FString& OutIPorDomain)
{
	FString L;
	URL.Split(TEXT("//"), &L, &OutIPorDomain);
}

void FGPTSoVITSManage::OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	int32 Code = HttpResponse->GetResponseCode();
	if (bSucceeded && Code == 200)
	{
		FString ProtocolString = HttpRequest->GetHeader(TEXT("Protocol"));

		FString Msg = HttpResponse->GetContentAsString();
		
		/*
		if (ProtocolString.Equals(TEXT("SpeechSynthesis")))
		{
			//response需要修改  Manage/Json处
			FGPTSoVITSSynthesisResponse Response;
			GPTSoVITSJson::JsonStringToSpeechSynthesisResponse(Msg, Response);


			OnGPTSoVITSSynthesisResponseDelegate.ExecuteIfBound(bSucceeded,Code,Response);
		}
		*/
	}
	else
	{

	}
}

void FGPTSoVITSManage::OnRequestProgress(FHttpRequestPtr HttpRequest, int32 SendBytes, int32 BytesReceived)
{
}

void FGPTSoVITSManage::OnRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
{
}

void FGPTSoVITSManage::OnMessage(const FString& InMsg)
{
}

void FGPTSoVITSManage::OnMessagesent(const FString& InMsg)
{
}

void FGPTSoVITSManage::OnClosed(int32 InStatusCode, const FString& InReason, bool bWasClean)
{
}

void FGPTSoVITSManage::OnConnectionError(const FString& InError)
{
}

void FGPTSoVITSManage::OnConnected()
{
}

void FGPTSoVITSManage::OnRawMessage(const void* InData, SIZE_T InSize, SIZE_T InBytesRemaining)
{
}
