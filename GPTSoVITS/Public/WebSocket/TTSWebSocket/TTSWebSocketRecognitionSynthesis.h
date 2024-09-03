#pragma once

#include "WebSocket/TTSWebSocket/Core/TTSWebSocketCore.h"

class FTTSWebSocketRecognitionSynthesis :public FTTSWebSocketCore
{
protected:
	virtual void OnMessage(const FString& InMsg);
	virtual void OnMessageSent(const FString& InMsg);
	virtual void OnClosed(int32 InStatusCode, const FString& InReason, bool bWasClean);
	virtual void OnConnectionError(const FString& InError);
	virtual void OnConnected();
	virtual void OnRawMessage(const void* InData, SIZE_T InSize, SIZE_T InBytesRemaining);

};