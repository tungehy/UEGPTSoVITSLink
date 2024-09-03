#include "WebSocket/GPTSoVITSWebSocket.h"

FGPTSoVITSWebSocket::FGPTSoVITSWebSocket()
{
	
}

FGPTSoVITSWebSocket::FGPTSoVITSWebSocket(const FString& InURL)
{
	URL = InURL;
}

void FGPTSoVITSWebSocket::Init(const FString& InURL)
{
	URL = InURL;
	if (Socket)
	{
		Unlink();
	}
}

void FGPTSoVITSWebSocket::Link()
{
	if (Socket)
	{
		Socket->Connect();
	}
}

void FGPTSoVITSWebSocket::Unlink()
{
	if (Socket)
	{
		Socket->OnConnected().RemoveAll(this); 
		Socket->OnConnected().Clear();

		Socket->OnConnectionError().RemoveAll(this); 
		Socket->OnConnectionError().Clear();

		Socket->OnRawMessage().RemoveAll(this); 
		Socket->OnRawMessage().Clear();

		Socket->OnMessageSent().RemoveAll(this); 
		Socket->OnMessageSent().Clear();

		Socket->OnMessage().RemoveAll(this); 
		Socket->OnMessage().Clear();

		Socket->OnClosed().RemoveAll(this); 
		Socket->OnClosed().Clear();

		if (Socket->IsConnected())
		{
			Socket->Close();
		}
	}
	Socket = nullptr; 
}

void FGPTSoVITSWebSocket::Relink()
{
	BuildWebSocket(URL);

	Link();
}

void FGPTSoVITSWebSocket::Send(const FString& InMsg)
{
	if (Socket->IsConnected())
	{
		Socket->Send(InMsg);
	}
}

void FGPTSoVITSWebSocket::Send(const TArray<uint8>& InBytes)
{
	Send((uint8*)InBytes.GetData(), sizeof(uint8) * InBytes.Num());
	
}

void FGPTSoVITSWebSocket::Send(uint8* InPtr, int32 InNum)
{
	if (Socket->IsConnected())
	{
		Socket->Send(InPtr, InNum);
	}
}

bool FGPTSoVITSWebSocket::IsConnected()
{
	if (Socket)
	{
		return Socket->IsConnected();
	}
	return false;
}

void FGPTSoVITSWebSocket::BuildWebSocket(const FString& InURL, const FString& InProtocol)
{
	Socket = FWebSocketsModule::Get().CreateWebSocket(InURL, InProtocol);

	Socket->OnConnected().AddLambda([&]()
	{
		OnConnectedDelegate.ExecuteIfBound();
	});

	Socket->OnConnectionError().AddLambda([&](const FString& Error)
	{
		OnConnectionErrorDelegate.ExecuteIfBound(Error);
	});

	Socket->OnClosed().AddLambda([&](int32 InStatusCode, const FString& InReason, bool bWasClean)
	{
		OnClosedDelegate.ExecuteIfBound(InStatusCode, InReason, bWasClean);
	});

	Socket->OnRawMessage().AddLambda([&](const void* InData, SIZE_T InSize, SIZE_T InBytesRemaining)
	{
		OnRawMessageDelegate.ExecuteIfBound(InData, InSize, InBytesRemaining);
	});

	Socket->OnMessageSent().AddLambda([&](const FString& InMessageString)
	{
		OnMessageSentDelegate.ExecuteIfBound(InMessageString);
	});

	Socket->OnMessage().AddLambda([&](const FString& InMessage)
	{
		OnMessageSentDelegate.ExecuteIfBound(InMessage);
	});
	
}
