#pragma once

#include "CoreMinimal.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "GPTSoVITSType.h"

class FGPTSoVITSWebSocket :public TSharedFromThis<FGPTSoVITSWebSocket>
{
public:
	FSimpleDelegate OnConnectedDelegate;
	FGPTSoVITSConnectionErrorDelegate OnConnectionErrorDelegate;
	FGPTSoVITSClosedDelegate OnClosedDelegate;
	FGPTSoVITSRawMessageDelegate OnRawMessageDelegate;
	FGPTSoVITSMessageSentDelegate OnMessageSentDelegate;
	FGPTSoVITSMessageDelegate OnMessageDelegate;

public:
	FGPTSoVITSWebSocket();

	FGPTSoVITSWebSocket(const FString &InURL);

public:
	void Init(const FString &InURL);

	void Link();
	void Unlink();
	void Relink();

public:
	void Send(const FString& InMsg);
	void Send(const TArray<uint8>& inBytes);
	void Send(uint8* InPtr, int32 InNum);

public:
	bool IsConnected();

protected:
	void BuildWebSocket(const FString& InURL = TEXT("ws://(addr)"), const FString& InProtocol = TEXT(""));

private:
	TSharedPtr<IWebSocket> Socket;
	FString URL;
};