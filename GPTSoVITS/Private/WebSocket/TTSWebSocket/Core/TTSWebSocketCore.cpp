#include "WebSocket/TTSWebSocket/Core/TTSWebSocketCore.h"

FTTSWebSocketCore::FTTSWebSocketCore()
	:Event(FPlatformProcess::GetSynchEventFromPool())
{
	WebSocket = MakeShareable(new FGPTSoVITSWebSocket);

	WebSocket->OnClosedDelegate.BindRaw(this, &FTTSWebSocketCore::OnClosed); 
	WebSocket->OnConnectedDelegate.BindRaw(this, &FTTSWebSocketCore::OnConnected); 
	WebSocket->OnConnectionErrorDelegate.BindRaw(this, &FTTSWebSocketCore::OnConnectionError); 
	WebSocket->OnMessageSentDelegate.BindRaw(this, &FTTSWebSocketCore::OnMessageSent);
	WebSocket->OnMessageDelegate.BindRaw(this, &FTTSWebSocketCore::OnMessage); 
	WebSocket->OnRawMessageDelegate.BindRaw(this, &FTTSWebSocketCore::OnRawMessage);

	StopThread = false;
}

FTTSWebSocketCore::~FTTSWebSocketCore()
{
	FPlatformProcess::ReturnSynchEventToPool(Event);
}

void FTTSWebSocketCore::Init(const FString& InURL)
{
	WebSocket->Init(InURL);
}

void FTTSWebSocketCore::Link()
{
	WebSocket->Link();
}

void FTTSWebSocketCore::Unlink()
{
	WebSocket->Unlink();
}

void FTTSWebSocketCore::Relink()
{
	WebSocket->Relink();
}

bool FTTSWebSocketCore::IsConnected()
{
	return WebSocket->IsConnected();
}

void FTTSWebSocketCore::OnMessage(const FString& InMsg)
{
}

void FTTSWebSocketCore::OnMessageSent(const FString& InMsg)
{
}

void FTTSWebSocketCore::OnClosed(int32 InStatusCode, const FString& InReason, bool bWasClean)
{
}

void FTTSWebSocketCore::OnConnectionError(const FString& InError)
{
}

void FTTSWebSocketCore::OnConnected()
{
}

void FTTSWebSocketCore::OnRawMessage(const void* InData, SIZE_T InSize, SIZE_T InBytesRemaining)
{
}
