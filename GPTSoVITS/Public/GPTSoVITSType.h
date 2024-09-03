#pragma once


#include "CoreMinimal.h"
#include "GPTSoVITSType.generated.h"


/*json示例
{
	"audio":"exsi...",
	"audio_format":"wav",
	"sample_rate":"16000",
	"lang":"zh_cn",
	"punc":"0"        是否开启标点符号
}
*/

//语音识别请求
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

/*响应格式
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

	//请求成功与否
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Responce")
	bool bSuccess;

	//错误码 success为true时，code=0；success为false时，code为具体错误码
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Responce")
	int32 Code;

	//描述错误信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Responce")
	FGPTSoVITSMessage Message;

	//一次请求的唯一标识，作trace用
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


//语音识别响应
USTRUCT(BlueprintType)
struct GPTSOVITS_API FGPTSoVITSRecognitionResponse :public FGPTSoVITSResponse
{
	GENERATED_USTRUCT_BODY()

	FGPTSoVITSRecognitionResponse();

	//asr识别结果
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|RecognitionResponse")
	FGPTSoVITSRecognitionResult Result;
};


/*api_V3
GET:
```
http://127.0.0.1:9880/tts?text=先帝创业未半而中道崩殂，今天下三分，益州疲弊，此诚危急存亡之秋也。&text_lang=zh&ref_audio_path=archive_jingyuan_1.wav&prompt_lang=zh&prompt_text=我是「罗浮」云骑将军景元。不必拘谨，「将军」只是一时的身份，你称呼我景元便可&text_split_method=cut5&batch_size=1&media_type=wav&streaming_mode=true
```

POST:
```json
{
	"text": "",                                                 # str.(required必填) text to be synthesized
	"text_lang": "",                                            # str.(required) language of the text to be synthesized
	"ref_audio_path": "",                                       # str.(required) reference audio path.
	"prompt_text": "",                                          # str.(optional可选) prompt text for the reference audio
	"prompt_lang": "",                                          # str.(required) language of the prompt text for the reference audio
	"top_k": 5,                                                 # int.(optional) 前k个抽样
	"top_p": 1,                                                 # float.(optional) 顶部p采样
	"temperature": 1,                                           # float.(optional) temperature for sampling
	"text_split_method": "cut5",                                # str.(optional) 文本分割方法, see text_segmentation_method.py for details.
	"batch_size": 1,                                            # int.(optional) 用于推理的批量大小
	"batch_threshold": 0.75,                                    # float.(optional) 批量分割阈值.
	"split_bucket": true,                                       # bool.(optional) 是否将批次分成多桶.
	"speed_factor":1.0,                                         # float.(optional) 控制合成后的音频速率.
	"fragment_interval":0.3,                                    # float.(optional) 控制音频片段间隔.
	"seed": -1,                                                 # int.(optional) 可重现的随机种子.
	"media_type": "wav",                                        # str.(optional) 输出音频类型 support "wav", "raw", "ogg", "aac".
	"streaming_mode": false,                                    # bool.(optional) 是否返回流式响应.
	"parallel_infer": True,                                     # bool.(optional) 是否使用并行推理.
	"repetition_penalty": 1.35,                                 # float.(optional) T2S模型的重复惩罚.
	"tts_infer_yaml_path": “GPT_SoVITS/configs/tts_infer.yaml”  # str.(optional) tts 推断 yaml 路径
}
*/

//GPTSoVITS语音合成   Param参数基类封装

USTRUCT(BlueprintType)
struct GPTSOVITS_API FGPTSoVITSSynthesisParam
{
	GENERATED_USTRUCT_BODY()

	FGPTSoVITSSynthesisParam();

	//前k个抽样,选填
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	int32 TopK;

	//顶部p采样，选填
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	float TopP;

	//采样温度，选填 ，用于控制模型训练过程中参数更新的频率或效率，通常情采样温度越高，生成的样本越具有多样性，相反，采样温度较低则更倾向于选择概率较高的输出
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	float Temperature;

	//文本切割方法,选填，默认按标点符号切
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	FString TextSplitMethod;

	//用于推理的批量大小,选填
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	int32 BatchSize;

	//批量分割阈值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	float BatchThreshold;

	//是否将批次分成多桶
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	bool SplitBucket;

	//控制合成后的音频速率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	float SpeedFactor;

	//控制音频片段间隔
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	float FragmentInterval;

	//可重现的随机种子
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	int32 Seed;

	//输出音频类型 "wav", "raw", "ogg", "aac"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	FString MediaType;

	//是否返回流式响应
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	bool StreamingMode;

	//是否使用并行推理
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	bool ParallelInfer;

	//T2S模型的重复惩罚
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	float RepetitionPenalty;

	//tts 推断的 yaml 路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis|Param")
	FString ttsInferYamlPath;

};

USTRUCT(BlueprintType)
struct GPTSOVITS_API FGPTSoVITSSynthesisRequest :public FGPTSoVITSSynthesisParam
{
	GENERATED_USTRUCT_BODY()

	FGPTSoVITSSynthesisRequest();

	//发音人id,为后续动态修改模型，或者，开启多个接口进行切换，来满足不同的朗读者切换请求
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis")
	//int32 SpkId;

	//需要TTS的文本
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis")
	FString OutputText;

	//需要TTS的文本所使用语言
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis")
	FString OutputLanguage;
	
	//参考音频路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis")
	FString refAudioPath;

	//参考音频的标注
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis")
	FString PromptText;

	//参考音频的语言
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSoVITS|Synthesis")
	FString PromptLanguage;
};

/*
//response需要修改
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
DECLARE_DELEGATE_ThreeParams(FGPTSoVITSRawMessageDelegate, const void*, SIZE_T, SIZE_T);//原生数据的接收
DECLARE_DELEGATE_OneParam(FGPTSoVITSMessageSentDelegate, const FString&);
DECLARE_DELEGATE_OneParam(FGPTSoVITSMessageDelegate, const FString&);


//tts服务器
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

//tts服务器 响应
USTRUCT(BlueprintType)
struct GPTSOVITS_API FWSGPTSOVITSSynthesisTTSResponse
{
	GENERATED_USTRUCT_BODY()

	FWSGPTSOVITSSynthesisTTSResponse();

	//状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPTSOVITS|TTSResponse|Synthesis");
	int32 Status;

	//信号
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "GPTSOVITS|TTSResponse|Synthesis");
	FString Signal;

	//返回的结果
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "GPTSOVITS|TTSResponse|Synthesis");
	FString Audio;

	//
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "GPTSOVITS|TTSResponse|Synthesis");
	FString Session;
};

//链接请求
USTRUCT(BlueprintType)
struct GPTSOVITS_API FWSGPTSOVITSSynthesisActionTTSRequest
{
	GENERATED_USTRUCT_BODY()

	FWSGPTSOVITSSynthesisActionTTSRequest();

	//任务
	UPROPERTY(Editanywhere, BlueprintReadwrite, Category = "GPTSOVITS|ActionTTSRequest|Synthesis");
	FString Task;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "GPTSOVITS|ActionTTSRequest|Synthesis");
	FString Signal;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "GPTSOVITS|ActionTTSRequest|Synthesis");
	FString Session;
};
