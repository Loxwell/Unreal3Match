#pragma once

#include "kismet/BlueprintFunctionLibrary.h"
#include "Match3BPFunctionLibrary.generated.h"

UCLASS()
class PROJECT3MATCH_API UMatch3BPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = GamePlay, meta = (WorldContext = "WorldContextObject"))
		static APlayerController* GetLocalPlayerController(UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category = GamePlay, meta = (WorldContext = "WorldContextObject"))
		static FString GetOnlineAccountID(APlayerController* PlayerController);
	UFUNCTION(BlueprintCallable, Category = GamePlay, meta = (WorldContext = "WorldContextObject"))
		static bool IsGameActive(UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category = GamePlay, meta = (WorldContext = "WorldContextObject"))
		static void PauseGameTimer(UObject* WorldContextObject, bool bPause);
};