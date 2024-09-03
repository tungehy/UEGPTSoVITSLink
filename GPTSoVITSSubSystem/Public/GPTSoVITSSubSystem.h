#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GPTSoVITSType.h"
#include "GPTSoVITSSubSystem.generated.h"

//response相关内容
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGPTSoVITSSynthesisResponseBPDelegate, bool, bSucceeded, int32, InCode,const FGPTSoVITSSynthesisResponse&, InGPTSoVITSSynthesisResponse);

UCLASS(BlueprintType)
class GPTSOVITSSUBSYSTEM_API UGPTSoVITSSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGPTSoVITSSubSystem();

/*
public:
	UPROPERTY(BlueprintAssignable)
	FGPTSoVITSSynthesisResponseBPDelegate GPTSoVITSSynthesisResponseDelegate;
*/

public:
	UFUNCTION(BlueprintCallable, Category = "GPTSoVITSSubSystem|Http")
	void InitBindResponseDelegate();

	UFUNCTION(BlueprintCallable, Category = "GPTSoVITSSubSystem|Http")
	void CancelRequest(const FGuid& InRequestID);

	UFUNCTION(BlueprintCallable, Category = "GPTSoVITSSubSystem|Http")
	void SetURL(const FString& InURL);

	UFUNCTION(BlueprintPure, Category = "GPTSoVITSSubSystem|Http")
	FString GetURL() const;

	//这个请求是否在请求当中
	UFUNCTION(BlueprintPure, Category = "GPTSoVITSSubSystem|Http")
	bool IsValidRequest(const FGuid& InRequestID) const;

public:
	UFUNCTION(BlueprintCallable, Category = "GPTSoVITSSubSystem|WebSocket")
	void Link();

	UFUNCTION(BlueprintCallable, Category = "GPTSoVITSSubSystem|WebSocket")
	void Relink();

	UFUNCTION(BlueprintCallable, Category = "GPTSoVITSSubSystem|WebSocket")
	void Unlink();

	UFUNCTION(BlueprintPure, Category = "GPTSoVITSSubSystem|WebSocket")
	bool IsConnected();

public:
	UFUNCTION(BlueprintCallable, Category = "GPTSoVITSSubSystem")
	FGuid SpeechSynthesis(const FGPTSoVITSSynthesisRequest& InRequest);
};