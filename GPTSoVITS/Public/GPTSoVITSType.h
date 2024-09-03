#pragma once


#include "CoreMinimal.h"
#include "GPTSoVITSType.generated.h"


/*jsonʾ��
{
	"audio":"exsi...",
	"audio_format":"wav",
	"sample_rate":"16000",
	"lang":"zh_cn",
	"punc":"0"        �Ƿ���������
}
*/

//����ʶ������
USTRUCT(BlueprintType)
struct GPTSOVITS_API FGPTSoVITSRecognitionRequest
{
	GENERATED_USTRUCT_BODY()

	FGPTSoVITSRecognitionRequest();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Recognition")
	FString Audio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Recognition")
	FString AudioFormat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Recognition")
	int32 SampleRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Recognition")
	FString Lang;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Recognition")
	bool bPunc;
};

/*��Ӧ��ʽ
{
	"success": true,
	"code": 0,
	"message": {"global":"success"},
	"result": {
		"description":"",
		"input": <sentence to be synthesized>,
		"output":<wavfile>
	}
}
*/
USTRUCT(BlueprintType)
struct GPTSOVITS_API FGPTSoVITSMessage
{
	GENERATED_USTRUCT_BODY()

	FGPTSoVITSMessage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Message")
	FString Global;
};

USTRUCT(BlueprintType)
struct GPTSOVITS_API FGPTSoVITSResponse
{
	GENERATED_USTRUCT_BODY()

	FGPTSoVITSResponse();

	//����ɹ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Responce")
	bool bSuccess;

	//������ successΪtrueʱ��code=0��successΪfalseʱ��codeΪ���������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Responce")
	int32 Code;

	//����������Ϣ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Responce")
	FGPTSoVITSMessage Message;

	//һ�������Ψһ��ʶ����trace��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Responce")
	FString RequestId;
};

USTRUCT(BlueprintType)
struct GPTSOVITS_API FGPTSoVITSRecognitionResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Result")
	FString Transcription;
};


//����ʶ����Ӧ
USTRUCT(BlueprintType)
struct GPTSOVITS_API FGPTSoVITSRecognitionResponse :public FGPTSoVITSResponse
{
	GENERATED_USTRUCT_BODY()

	FGPTSoVITSRecognitionResponse();

	//asrʶ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|RecognitionResponse")
	FGPTSoVITSRecognitionResult Result;
};


/*api_V3
GET:
```
http://127.0.0.1:9880/tts?text=�ȵ۴�ҵδ����е����㣬���������֣�����ƣ�ף��˳�Σ������֮��Ҳ��&text_lang=zh&ref_audio_path=archive_jingyuan_1.wav&prompt_lang=zh&prompt_text=���ǡ��޸������ｫ����Ԫ�����ؾн�����������ֻ��һʱ����ݣ���ƺ��Ҿ�Ԫ���&text_split_method=cut5&batch_size=1&media_type=wav&streaming_mode=true
```

POST:
```json
{
	"text": "",                                                 # str.(required����) text to be synthesized
	"text_lang": "",                                            # str.(required) language of the text to be synthesized
	"ref_audio_path": "",                                       # str.(required) reference audio path.
	"prompt_text": "",                                          # str.(optional��ѡ) prompt text for the reference audio
	"prompt_lang": "",                                          # str.(required) language of the prompt text for the reference audio
	"top_k": 5,                                                 # int.(optional) ǰk������
	"top_p": 1,                                                 # float.(optional) ����p����
	"temperature": 1,                                           # float.(optional) temperature for sampling
	"text_split_method": "cut5",                                # str.(optional) �ı��ָ��, see text_segmentation_method.py for details.
	"batch_size": 1,                                            # int.(optional) ���������������С
	"batch_threshold": 0.75,                                    # float.(optional) �����ָ���ֵ.
	"split_bucket": true,                                       # bool.(optional) �Ƿ����ηֳɶ�Ͱ.
	"speed_factor":1.0,                                         # float.(optional) ���ƺϳɺ����Ƶ����.
	"fragment_interval":0.3,                                    # float.(optional) ������ƵƬ�μ��.
	"seed": -1,                                                 # int.(optional) �����ֵ��������.
	"media_type": "wav",                                        # str.(optional) �����Ƶ���� support "wav", "raw", "ogg", "aac".
	"streaming_mode": false,                                    # bool.(optional) �Ƿ񷵻���ʽ��Ӧ.
	"parallel_infer": True,                                     # bool.(optional) �Ƿ�ʹ�ò�������.
	"repetition_penalty": 1.35,                                 # float.(optional) T2Sģ�͵��ظ��ͷ�.
	"tts_infer_yaml_path": ��GPT_SoVITS/configs/tts_infer.yaml��  # str.(optional) tts �ƶ� yaml ·��
}
*/

//GPTSoVITS�����ϳ�   Param���������װ

USTRUCT(BlueprintType)
struct GPTSOVITS_API FGPTSoVITSSynthesisParam
{
	GENERATED_USTRUCT_BODY()

	FGPTSoVITSSynthesisParam();

	//ǰk������,ѡ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	int32 TopK;

	//����p������ѡ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	float TopP;

	//�����¶ȣ�ѡ�� �����ڿ���ģ��ѵ�������в������µ�Ƶ�ʻ�Ч�ʣ�ͨ��������¶�Խ�ߣ����ɵ�����Խ���ж����ԣ��෴�������¶Ƚϵ����������ѡ����ʽϸߵ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	float Temperature;

	//�ı��и��,ѡ�Ĭ�ϰ���������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	FString TextSplitMethod;

	//���������������С,ѡ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	int32 BatchSize;

	//�����ָ���ֵ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	float BatchThreshold;

	//�Ƿ����ηֳɶ�Ͱ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	bool SplitBucket;

	//���ƺϳɺ����Ƶ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	float SpeedFactor;

	//������ƵƬ�μ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	float FragmentInterval;

	//�����ֵ��������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	int32 Seed;

	//�����Ƶ���� "wav", "raw", "ogg", "aac"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	FString MediaType;

	//�Ƿ񷵻���ʽ��Ӧ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	bool StreamingMode;

	//�Ƿ�ʹ�ò�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	bool ParallelInfer;

	//T2Sģ�͵��ظ��ͷ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	float RepetitionPenalty;

	//tts �ƶϵ� yaml ·��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	FString ttsInferYamlPath;

};

USTRUCT(BlueprintType)
struct GPTSOVITS_API FGPTSoVITSSynthesisRequest :public FGPTSoVITSSynthesisParam
{
	GENERATED_USTRUCT_BODY()

	FGPTSoVITSSynthesisRequest();

	//������id,Ϊ������̬�޸�ģ�ͣ����ߣ���������ӿڽ����л��������㲻ͬ���ʶ����л�����
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis")
	//int32 SpkId;

	//��ҪTTS���ı�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis")
	FString OutputText;

	//��ҪTTS���ı���ʹ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis")
	FString OutputLanguage;
	
	//�ο���Ƶ·��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis")
	FString refAudioPath;

	//�ο���Ƶ�ı�ע
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis")
	FString PromptText;

	//�ο���Ƶ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis")
	FString PromptLanguage;
};

/*
//response��Ҫ�޸�
USTRUCT(BlueprintType)
struct GPTSOVITS_API FGPTSoVITSSynthesisResponse
{
	GENERATED_USTRUCT_BODY()

	FGPTSoVITSSynthesisResponse();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis")
	FString Aaa;

};
*/

//DECLARE_DELEGATE_ThreeParams(FGPTSoVITSSynthesisResponseDelegate,bool, int32, const FGPTSoVITSSynthesisResponse&)

DECLARE_DELEGATE_OneParam(FGPTSoVITSConnectionErrorDelegate, const FString&);
DECLARE_DELEGATE_ThreeParams(FGPTSoVITSClosedDelegate, int32, const FString&, bool);
DECLARE_DELEGATE_ThreeParams(FGPTSoVITSRawMessageDelegate, const void*, SIZE_T, SIZE_T);//ԭ�����ݵĽ���
DECLARE_DELEGATE_OneParam(FGPTSoVITSMessageSentDelegate, const FString&);
DECLARE_DELEGATE_OneParam(FGPTSoVITSMessageDelegate, const FString&);


//tts������
USTRUCT(BlueprintType)
struct GPTSOVITS_API FWSGPTSOVITSSynthesisTTSRequest
{
	GENERATED_USTRUCT_BODY()

	FWSGPTSOVITSSynthesisTTSRequest();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSOVITSTTS|Request|Synthesis");
	FString Text;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "GPTSOVITSTTS|TTSResponse|Synthesis");
	int32 SpkId;
};

//tts������ ��Ӧ
USTRUCT(BlueprintType)
struct GPTSOVITS_API FWSGPTSOVITSSynthesisTTSResponse
{
	GENERATED_USTRUCT_BODY()

	FWSGPTSOVITSSynthesisTTSResponse();

	//״̬
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSOVITS|TTSResponse|Synthesis");
	int32 Status;

	//�ź�
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "GPTSOVITS|TTSResponse|Synthesis");
	FString Signal;

	//���صĽ��
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "GPTSOVITS|TTSResponse|Synthesis");
	FString Audio;

	//
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "GPTSOVITS|TTSResponse|Synthesis");
	FString Session;
};

//��������
USTRUCT(BlueprintType)
struct GPTSOVITS_API FWSGPTSOVITSSynthesisActionTTSRequest
{
	GENERATED_USTRUCT_BODY()

	FWSGPTSOVITSSynthesisActionTTSRequest();

	//����
	UPROPERTY(Editanywhere, BlueprintReadwrite, Category = "GPTSOVITS|ActionTTSRequest|Synthesis");
	FString Task;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "GPTSOVITS|ActionTTSRequest|Synthesis");
	FString Signal;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "GPTSOVITS|ActionTTSRequest|Synthesis");
	FString Session;
};
