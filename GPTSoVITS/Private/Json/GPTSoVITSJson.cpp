#include "Json/GPTSoVITSJson.h"
#include "Json.h"

namespace GPTSoVITSJson
{
	
	//不需要内部通信的放到外部即可
	//通用响应
	void JsonObjectToGPTSoVITSReponse(const TSharedPtr<FJsonObject> InJsonObject, FGPTSoVITSResponse* InResponse);
	//语音合成通用参数
	void SpeechSynthesisParamToJsonString(const TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter, const FGPTSoVITSSynthesisParam* InParam);
	//void JsonObjectToSpeechSynthesisParam(const TSharedPtr<FJsonObject> InJsonObject, FGPTSoVITSSynthesisParam* InParam);


	void SpeechSynthesisParamToJsonString(const TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter, const FGPTSoVITSSynthesisParam* InParam)
	{
		if (JsonWriter && InParam)
		{
			JsonWriter->WriteValue(TEXT("top_k"), InParam->TopK);
			JsonWriter->WriteValue(TEXT("top_p"), InParam->TopP);
			JsonWriter->WriteValue(TEXT("temperature"), InParam->Temperature);
			JsonWriter->WriteValue(TEXT("text_split_method"), InParam->TextSplitMethod);
			JsonWriter->WriteValue(TEXT("batch_size"), InParam->BatchSize);
			JsonWriter->WriteValue(TEXT("batch_threshold"), InParam->BatchThreshold);
			JsonWriter->WriteValue(TEXT("split_bucket"), InParam->SplitBucket);
			JsonWriter->WriteValue(TEXT("speed_factor"), InParam->SpeedFactor);
			JsonWriter->WriteValue(TEXT("fragment_interval"), InParam->FragmentInterval);
			JsonWriter->WriteValue(TEXT("seed"), InParam->Seed);
			JsonWriter->WriteValue(TEXT("media_type"), InParam->MediaType);
			JsonWriter->WriteValue(TEXT("streaming_mode"), InParam->StreamingMode);
			JsonWriter->WriteValue(TEXT("parallel_infer"), InParam->ParallelInfer);
			JsonWriter->WriteValue(TEXT("repetition_penalty"), InParam->RepetitionPenalty);
			JsonWriter->WriteValue(TEXT("tts_infer_yaml_path"), InParam->ttsInferYamlPath);

		}
	}

	/*
	void JsonObjectToSpeechSynthesisParam(const TSharedPtr<FJsonObject> InJsonObject, FGPTSoVITSSynthesisParam* InParam)
	{
		if (InJsonObject && InParam)
		{
			InParam->TopK = InJsonObject->GetNumberField(TEXT("top_k"));
		}
	}
	*/

	void SpeechSynthesisRequestToJsonString(const FGPTSoVITSSynthesisRequest& InRequest, FString& OutJsonString)
	{
		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = 
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutJsonString);
		JsonWriter->WriteObjectStart();
		{
			JsonWriter->WriteValue(TEXT("text"), InRequest.OutputText);
			JsonWriter->WriteValue(TEXT("text_lang"), InRequest.OutputLanguage);
			JsonWriter->WriteValue(TEXT("ref_audio_path"), InRequest.refAudioPath);
			JsonWriter->WriteValue(TEXT("prompt_text"), InRequest.PromptText);
			JsonWriter->WriteValue(TEXT("prompt_lang"), InRequest.PromptLanguage);

			SpeechSynthesisParamToJsonString(JsonWriter, &InRequest);
		}
		JsonWriter->WriteObjectEnd();
		JsonWriter->Close();
	}

	void WSGPTSOVITSSynthesisTTSRequestToString(const FWSGPTSOVITSSynthesisTTSRequest& InRequest, FString& OutString)
	{
		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>>Jsonwriter = 
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutString);
		
		Jsonwriter->WriteObjectStart();//
		{
			Jsonwriter->WriteValue(TEXT("text"), InRequest.Text);
			Jsonwriter->WriteValue(TEXT("spk_id"), InRequest.SpkId);
		}

		Jsonwriter->WriteObjectEnd();
		Jsonwriter->Close();  //Json合并
	}

	void JsonStringToWSSpeechSynthesisResponse(const FString& InJsonString, FWSGPTSOVITSSynthesisTTSResponse& OutResponse)
	{
		TSharedRef<TJsonReader<>>JsonReader = TJsonReaderFactory<>::Create(InJsonString); 
		TSharedPtr<FJsonValue> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
		{ 
			if (TSharedPtr<FJsonObject>InJsonObject = ReadRoot->AsObject())
			{
				OutResponse.Status = InJsonObject->GetIntegerField(TEXT("status"));
				OutResponse.Signal = InJsonObject->GetStringField(TEXT("signal"));
				OutResponse.Audio = InJsonObject->GetStringField(TEXT("audio"));
				OutResponse.Session = InJsonObject->GetStringField(TEXT("session"));
			}
		}
	}

	void WSGPTSOVITSSynthesisTTSRequestToString(const FWSGPTSOVITSSynthesisActionTTSRequest& InRequest, FString& OutString)
	{
		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>>Jsonwriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutString);

		Jsonwriter->WriteObjectStart();//
		{
			Jsonwriter->WriteValue(TEXT("task"), InRequest.Task);
			Jsonwriter->WriteValue(TEXT("signal"), InRequest.Signal);
			if (!InRequest.Session.IsEmpty())
			{
				Jsonwriter->WriteValue(TEXT("session"), InRequest.Session);
			}
			
		}

		Jsonwriter->WriteObjectEnd();
		Jsonwriter->Close();  //Json合并
	}

	/*
	//response需要修改
	void JsonStringToSpeechSynthesisResponse(const FString& InJsonString, const FGPTSoVITSSynthesisResponse& OutResponse)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonWriterFactory<>::Create(InJsonString);
		TSharedPtr<FJsonValue> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
		{
			if (TSharedPtr<FJsonObject>InJsonObject = ReadRoot->AsObject())
			{
				JsonObjectToGPTSoVITSReponse(InJsonObject, &OutResponse);
				OutResponse.Aaa = InJsonObject->GetStringField(TEXT("Aaa"));
			}
		}
	}
	*/
	
}