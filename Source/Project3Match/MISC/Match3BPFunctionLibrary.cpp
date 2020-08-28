#include "Match3BPFunctionLibrary.h"

APlayerController* UMatch3BPFunctionLibrary::GetLocalPlayerController(UObject* WorldContextObject)
{
	return nullptr;
}

FString UMatch3BPFunctionLibrary::GetOnlineAccountID(APlayerController* PlayerController)
{
	return FString("");
}

bool UMatch3BPFunctionLibrary::IsGameActive(UObject* WorldContextObject)
{
	return false;
}

void UMatch3BPFunctionLibrary::PauseGameTimer(UObject* WorldContextObject, bool bPause)
{

}