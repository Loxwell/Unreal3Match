#pragma once

#include "kismet/BlueprintFunctionLibrary.h"
#include "Match3BPFunctionLibrary.generated.h"

UCLASS()
class PROJECT3MATCH_API UMatch3BPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/// <summary>
	/// 게임 내 사용자 수만큼 PlayerController 생성 됨
	/// 네트워크 멀티 사용자 게임일 경우 로컬 플레이어만 검색 해서 반환함.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = GamePlay, meta = (WorldContext = "WorldContextObject"))
		static APlayerController* GetLocalPlayerController(UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category = GamePlay, meta = (WorldContext = "WorldContextObject"))
		static FString GetOnlineAccountID(APlayerController* PlayerController);
	UFUNCTION(BlueprintCallable, Category = GamePlay, meta = (WorldContext = "WorldContextObject"))
		static bool IsGameActive(UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category = GamePlay, meta = (WorldContext = "WorldContextObject"))
		static void PauseGameTimer(UObject* WorldContextObject, bool bPause);
};