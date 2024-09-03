#pragma once

#include "CoreMinimal.h"
#include "WebSocket/GPTSoVITSWebSocket.h"

class FEvent;

class FTTSWebSocketCore :public TSharedFromThis<FTTSWebSocketCore>
{
public:
	FTTSWebSocketCore();
	virtual ~FTTSWebSocketCore();     //~ ���������������ͷŸ���������������Դ     

public:
	//�麯����һ����������Ա�������������������н�����д
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
	//�����߳�
	TSharedPtr<FGPTSoVITSWebSocket>
		WebSocket;
	FEvent* Event;

	//��
	FCriticalSection Mutex;

	//ֹͣ�߳�
	bool StopThread;
};
