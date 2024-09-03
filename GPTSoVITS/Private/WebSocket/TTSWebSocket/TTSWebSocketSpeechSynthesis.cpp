#include "WebSocket/TTSWebSocket/TTSWebSocketSpeechSynthesis.h"
#include "Json/GPTSoVITSJson.h"
#include "GPTSoVITSManage.h"
#include "Async/Async.h"

FTTSWebSocketSpeechSynthesis::FTTSWebSocketSpeechSynthesis()
{
	SpkId = 0;
}

void FTTSWebSocketSpeechSynthesis::StartSpeechSynthesis(int32 InSpkid)
{
	StopThread = true;
	SpkId = InSpkid;
	if (WebSocket->IsConnected())
	{
		WebSocket->Unlink();
	}
	WebSocket->Relink();
}

void FTTSWebSocketSpeechSynthesis::SpeechSynthesis(const FString& InSynthesisContent)
{
	FScopeLock MutexLock(&Mutex);
	SynthesisBuffer.Add(InSynthesisContent);
}

void FTTSWebSocketSpeechSynthesis::EndSpeechSynthesis()
{
	if (!Session.IsEmpty())
	{
		Request(TEXT("end"), Session);

		Session = {}; 
		StopThread = true;
	}

}

void FTTSWebSocketSpeechSynthesis::Request(const TCHAR* InSignal, const FString& InSession)
{
	FWSGPTSOVITSSynthesisActionTTSRequest ActionTTSRequest;
	ActionTTSRequest.Signal = InSignal;
	ActionTTSRequest.Session = InSession;
	ActionTTSRequest.Task = TEXT("tts");
	
	FString ActionString; 
	GPTSoVITSJson::WSGPTSOVITSSynthesisTTSActionRequestToString(ActionTTSRequest, ActionString);
	
	WebSocket->Send(ActionString);
}

void FTTSWebSocketSpeechSynthesis::LoopSendBuffer_Asynchronous()
{
	while (!StopThread)
	{
		if (SynthesisBuffer.Num() > 0)
		{
			FScopeLock MutexLook(&Mutex);

			FWSGPTSOVITSSynthesisTTSRequest InTTSRequest;
			InTTSRequest.SpkId = SpkId;
			InTTSRequest.Text = SynthesisBuffer[0];

			SynthesisBuffer.RemoveAt(0);

			FString RequestString;
			GPTSoVITSJson::WSGPTSOVITSSynthesisTTSRequestToString(InTTSRequest, RequestString);

			WebSocket->Send(RequestString);
		}
		FPlatformProcess::Sleep(0.1f);
	}
}

void FTTSWebSocketSpeechSynthesis::OnMessage(const FString& InMsg)
{
}

void FTTSWebSocketSpeechSynthesis::OnMessageSent(const FString& InMsg)
{
	FWSGPTSOVITSSynthesisTTSResponse TTSResponse;
	GPTSoVITSJson::JsonStringToWSSpeechSynthesisResponse(InMsg, TTSResponse);

	if (TTSResponse.Signal.Equals(TEXT("server ready")))
	{
		Session = TTSResponse.Session;
		StopThread = false;

		AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask,
		[&]()
		{
			LoopSendBuffer_Asynchronous();
		});
	}
}

void FTTSWebSocketSpeechSynthesis::OnClosed(int32 InStatusCode, const FString& InReason, bool bWasClean)
{
}

void FTTSWebSocketSpeechSynthesis::OnConnectionError(const FString& InError)
{
}

void FTTSWebSocketSpeechSynthesis::OnConnected()
{
	Request(TEXT("start"));     //链接成功后发送start
}

void FTTSWebSocketSpeechSynthesis::OnRawMessage(const void* InData, SIZE_T InSize, SIZE_T InBytesRemaining)
{
	FString JsonString = ANSI_TO_TCHAR((ANSICHAR*)InData);

	FWSGPTSOVITSSynthesisTTSResponse Response; 
	GPTSoVITSJson::JsonStringToWSSpeechSynthesisResponse(JsonString, Response);

	if (Response.Audio.IsEmpty())
		{
			//执行上层代理
		}
}
