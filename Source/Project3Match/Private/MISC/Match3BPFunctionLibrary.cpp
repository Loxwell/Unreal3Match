#include "MISC/Match3BPFunctionLibrary.h"
#include "../Project3MatchGameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

#define GET_GAMEMODE(exp) Cast<AProject3MatchGameModeBase>(UGameplayStatics::GetGameMode((exp)))

APlayerController* UMatch3BPFunctionLibrary::GetLocalPlayerController(UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = Iterator->Get();
			// checks for client only
			// 클라이언트에 여러 PlayerController가 존재 가능함(클라이언트용, 서버용 다른 체험자용 PlayerController)
			if (PlayerController && PlayerController->IsLocalController())
				// For this project. 로컬 클라이언트는 사용자가 한명만 존재
				return PlayerController; 
		}
	}
	return nullptr;
}

FString UMatch3BPFunctionLibrary::GetOnlineAccountID(APlayerController* PlayerController)
{
	if (PlayerController && PlayerController->PlayerState && PlayerController->PlayerState->GetUniqueId().IsValid())
		return PlayerController->PlayerState->GetUniqueId()->GetHexEncodedString();
	return FString("");
}

bool UMatch3BPFunctionLibrary::IsGameActive(UObject* WorldContextObject)
{
	check(WorldContextObject);
	if (AProject3MatchGameModeBase* GameMode = GET_GAMEMODE(WorldContextObject))
		return GameMode->IsGameActive();
	return false;
}

void UMatch3BPFunctionLibrary::PauseGameTimer(UObject* WorldContextObject, bool bPause)
{
	check(WorldContextObject);
	if (AProject3MatchGameModeBase* GameMode = GET_GAMEMODE(WorldContextObject))
		GameMode->PauseGameTimer(bPause);
}