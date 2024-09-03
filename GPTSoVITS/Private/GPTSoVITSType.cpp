#include "GPTSoVITSType.h"

FGPTSoVITSRecognitionRequest::FGPTSoVITSRecognitionRequest()
{
	AudioFormat = TEXT("wav");
	SampleRate = 16000;
	Lang = TEXT("zh_cn");
	bPunc = true;
}

FGPTSoVITSMessage::FGPTSoVITSMessage()
{

}

FGPTSoVITSResponse::FGPTSoVITSResponse()
{
	bSuccess = false;
	Code = INDEX_NONE;
}

FGPTSoVITSRecognitionResponse::FGPTSoVITSRecognitionResponse()
{

}



FGPTSoVITSSynthesisParam::FGPTSoVITSSynthesisParam()
{
	TopK = 5;
	TopP = 1;
	Temperature = 1;
	TextSplitMethod = TEXT("cut5");
	BatchSize = 1;
	BatchThreshold = 0.75;
	SplitBucket = true;
	SpeedFactor = 1.0;
	FragmentInterval = 0.3;
	Seed = -1;
	MediaType = TEXT("wav");
	StreamingMode = false;
	ParallelInfer = true;
	RepetitionPenalty = 1.35;
	ttsInferYamlPath = TEXT("GPT_SoVITS/configs/tts_infer.yaml");
}

FGPTSoVITSSynthesisRequest::FGPTSoVITSSynthesisRequest()
{

}

FWSGPTSOVITSSynthesisTTSRequest::FWSGPTSOVITSSynthesisTTSRequest()
{
	SpkId =0;
}

FWSGPTSOVITSSynthesisActionTTSRequest::FWSGPTSOVITSSynthesisActionTTSRequest()
{
	Task=TEXT("tts");
}

FWSGPTSOVITSSynthesisTTSResponse::FWSGPTSOVITSSynthesisTTSResponse()
{

}
