#pragma once

#include "CoreMinimal.h"
#include "GPTSoVITSType.h"

namespace GPTSoVITSJson
{

	//�����ϳ�
	void GPTSOVITS_API SpeechSynthesisRequestToJsonString(const FGPTSoVITSSynthesisRequest& InRequest, FString &OutJsonString);
	//GPTSOVITSֱ�ӷ���wav
	//void GPTSOVITS_API JsonStringToSpeechSynthesisResponse(const FString& InJsonString,const FGPTSoVITSSynthesisResponse& OutResponse);

	///////////////////////WebSocket////////////////////////////
	//�����ϳ�(��)
	void GPTSOVITS_API WSGPTSOVITSSynthesisTTSRequestToString(const FWSGPTSOVITSSynthesisTTSRequest& InRequest, FString& OutString);
	void GPTSOVITS_API JsonStringToWSSpeechSynthesisResponse(const FString& InJsonString, FWSGPTSOVITSSynthesisTTSResponse& OutResponse);
	void GPTSOVITS_API WSGPTSOVITSSynthesisTTSActionRequestToString(const FWSGPTSOVITSSynthesisActionTTSRequest& InRequest, FString& OutString);


}