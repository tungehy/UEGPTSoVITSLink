#pragma once

#include "CoreMinimal.h"
#include "WebSocket/GPTSoVITSWebSocket.h"

class FEvent;

class FTTSWebSocketCore :public TSharedFromThis<FTTSWebSocketCore>
{
public:
	FTTSWebSocketCore();
	virtual ~FTTSWebSocketCore();     //~ 析构函数，负责释放该类对象所分配的资源     

public:
	//虚函数是一种特殊的类成员函数，允许在派生类中进行重写
	virtual void Init(const FString& InURL);
	
	virtual void Link();
	virtual void Unlink();
	virtual void Relink();
	virtual bool IsConnected();

protected:
	virtual void OnMessage(const FString& InMsg);
	virtual void OnMessageSent(const FString& InMsg); 
	virtual void OnClosed(int32 InStatusCode, const FString& InReason, bool bWasClean); 
	virtual void OnConnectionError(const FString& InError);
	virtual void OnConnected();
	virtual void OnRawMessage(const void* InData, SIZE_T InSize, SIZE_T InBytesRemaining);
	


protected:
	//挂起线程
	TSharedPtr<FGPTSoVITSWebSocket>
		WebSocket;
	FEvent* Event;

	//锁
	FCriticalSection Mutex;

	//停止线程
	bool StopThread;
};
