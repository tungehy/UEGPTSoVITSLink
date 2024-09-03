#pragma once


#include "CoreMinimal.h"
#include "HTTP/GPTSoVITSHTTP.h"
#include "WebSocket/GPTSoVITSWebSocket.h"
#include "WebSocket/TTSWebSocket/TTSWebSocketRecognitionSynthesis.h"
#include "WebSocket/TTSWebSocket/TTSWebSocketSpeechSynthesis.h"
#include "GPTSoVITSType.h"

//C++
class GPTSOVITS_API FGPTSoVITSManage
{
public:
	FGPTSoVITSManage();

public:
	static FGPTSoVITSManage* Get();
	static void Destory();

/*
public:
	FGPTSoVITSSynthesisResponseDelegate OnGPTSoVITSSynthesisResponseDelegate;//”Ô“Ù∫œ≥…œÏ”¶
*/

public:
	void Init();

public:
	FGuid SpeechSynthesis(const FString& InRequest);
	FGuid SpeechSynthesis(const FGPTSoVITSSynthesisRequest &InRequest);

	//HTTP
public:
	void CancelRequest(const FGuid& InRequestID);
	void SetURL(const FString& InURL);
	FString GetURL() const {return URL; };
	bool IsValidRequest(const FGuid& InRequestID) const;

	//WebSocket
public:
	void Link();
	void Relink(); 
	void Unlink(); 
	bool IsConnected();

public:
	void DebugCode(int32 InCode);
	void GetDomain(FString& OutIPorDomain);

	//HTTP
protected:
	void OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSsucceeded);
	void OnRequestProgress(FHttpRequestPtr HttpRequest, int32 SendBytes, int32 BytesReceived);
	void OnRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue);

	//WebSocket
protected:
	void OnMessage(const FString& InMsg);
	void OnMessagesent(const FString& InMsg);
	void OnClosed(int32 InStatusCode, const FString& InReason, bool bWasClean); 
	void OnConnectionError(const FString& InError);
	void OnConnected();
	void OnRawMessage(const void* InData, SIZE_T InSize, SIZE_T InBytesRemaining);

private:
	static FGPTSoVITSManage* Instance;

	TSharedPtr<GPTSoVITSHTTP::FHTTP> HTTP;
	TSharedPtr<FGPTSoVITSWebSocket> WebSocket;

	TSharedPtr<FTTSWebSocketRecognitionSynthesis> TTSWebSocketRecognitionSynthesisStreaming;
	TSharedPtr<FTTSWebSocketSpeechSynthesis> TTSWebSocketSpeechSynthesisStreaming;

	FString URL;
};