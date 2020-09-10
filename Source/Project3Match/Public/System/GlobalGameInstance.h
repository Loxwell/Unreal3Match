// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Match3GameInstance.h"
#include "../MISC/CollectionEnums.h"
#include "../MISC/SaveUserProperties.h"
#include "GlobalGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT3MATCH_API UGlobalGameInstance : public UMatch3GameInstance
{
	GENERATED_BODY()
	
public:
	UGlobalGameInstance() = default;

public:
	UPROPERTY(EditAnywhere)
		FSaveUserProperties SaveString;
	UPROPERTY(EditAnywhere)
		FUserStats Stats;
	UPROPERTY(EditAnywhere)
		FMedalValues Medals;
	UPROPERTY(EditAnywhere)
		FAchievements Achievements;
	UPROPERTY(EditAnywhere)
		FInAppPurchase InAppPurchase;
	UPROPERTY(EditAnywhere)
		FSettings Settings;
	UPROPERTY()
		class UUserWidget* StoredGameScreen;

public:
	UFUNCTION(BlueprintNativeEvent)
		void GameScreenResize();
	void GameScreenResize_Implementation() {
		UE_LOG(LogTemp, Warning, TEXT("블루 프린트에서 상세 구현"));
	}

	bool CheckAchievements(int32 StIndex, int32 EdIndex, int32 StatToCheck,const bool& bForceUpdate);
	float ChooseLocalOrServerAchievementValues(int32 Index, float RemoteValue);

protected:
	UFUNCTION(BlueprintImplementableEvent)
		void WriteAchievement(FName AchievementName, float Percent, int UserTag);

public:
	FVector2D StoredCameraRez;

private:
	FDateTime TimeEnteredBackground;
	FTimespan SessionDelayAllowed;
	
	FString CurrentPlatform;
	FString DebugString;
	FString IAPDebug;
	uint32 bIsLoggedIn : 1;
	uint32 bFirstLaunch : 1;
	EPlatformType Platform;
};
