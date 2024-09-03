#include "GPTSoVITSSubSystem.h"
#include "GPTSoVITSManage.h"


UGPTSoVITSSubSystem::UGPTSoVITSSubSystem()
{
	InitBindResponseDelegate();
}

void UGPTSoVITSSubSystem::InitBindResponseDelegate()
{
	/*
	FGPTSoVITSManage::Get()->OnGPTSoVITSSynthesisResponseDelegate.BindLambda(
		[&](bool bSucceeded, int32 InCode, const FGPTSoVITSSynthesisResponse& InResponse)
	{
		GPTSoVITSSynthesisResponseDelegate.Broadcast(bSucceeded, InCode, InResponse);
	});
	*/
}

void UGPTSoVITSSubSystem::CancelRequest(const FGuid& InRequestID)
{
	FGPTSoVITSManage::Get()->CancelRequest(InRequestID);
}

void UGPTSoVITSSubSystem::SetURL(const FString& InURL)
{
	FGPTSoVITSManage::Get()->SetURL(InURL);
}

FString UGPTSoVITSSubSystem::GetURL() const
{
	return FGPTSoVITSManage::Get()->GetURL();
}

bool UGPTSoVITSSubSystem::IsValidRequest(const FGuid& InRequestID) const
{
	return FGPTSoVITSManage::Get()->IsValidRequest(InRequestID);
}

void UGPTSoVITSSubSystem::Link()
{
	return FGPTSoVITSManage::Get()->Link();
}

void UGPTSoVITSSubSystem::Relink()
{
	return FGPTSoVITSManage::Get()->Relink();
}

void UGPTSoVITSSubSystem::Unlink()
{
	return FGPTSoVITSManage::Get()->Unlink();
}

bool UGPTSoVITSSubSystem::IsConnected()
{
	return FGPTSoVITSManage::Get()->IsConnected();
}

FGuid UGPTSoVITSSubSystem::SpeechSynthesis(const FGPTSoVITSSynthesisRequest& InRequest)
{
	return FGPTSoVITSManage::Get()->SpeechSynthesis(InRequest);
}
