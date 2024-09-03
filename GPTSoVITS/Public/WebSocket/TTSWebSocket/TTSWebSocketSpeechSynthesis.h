#pragma once

#include "WebSocket/TTSWebSocket/Core/TTSWebSocketCore.h"

class FTTSWebSocketSpeechSynthesis :public FTTSWebSocketCore
{
public:
	FTTSWebSocketSpeechSynthesis();

//流式语音合成
public:
	void StartSpeechSynthesis(int32 InSpkid);
	void SpeechSynthesis(const FString& InSynthesisContent);
	void EndSpeechSynthesis();

public:
	void Request(const TCHAR* Insiqnal, const FString& Insession = TEXT(""));

protected:
	void LoopSendBuffer_Asynchronous();

protected:
	virtual void OnMessage(const FString& InMsg);
	virtual void OnMessageSent(const FString& InMsg);
	virtual void OnClosed(int32 InStatusCode, const FString& InReason, bool bWasClean);
	virtual void OnConnectionError(const FString& InError);
	virtual void OnConnected();
	virtual void OnRawMessage(const void* InData, SIZE_T InSize, SIZE_T InBytesRemaining);

protected:
	FString Session;
	int32 SpkId;


	TArray<FString> SynthesisBuffer; //语句池，会不断发送到服务器上

};