#pragma once

#include "SaveUserProperties.generated.h"

USTRUCT(BlueprintType)
struct FSaveUserProperties
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGameKey)
		FString PlaySounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGameKey)
		FString PlayMusic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGameKey)
		FString PlayFeedback;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGameKey)
		FString PlayShake;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGameKey)
		FString GamesWon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGameKey)
		FString GamesPlayed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGameKey)
		FString GoldsWon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGameKey)
		FString BronzesWon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGameKey)
		FString PurchasedTheme;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGameKey)
		FString HighScore;
};

USTRUCT(BlueprintType)
struct FUserStats 
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UserStat)
		int32 CurrentGamesWon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UserStat)
		int32 CurrentGamesPlayed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UserStat)
		int32 CurrentSilversWon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UserStat)
		int32 CurrentBronzesWon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UserStat)
		int32 HighScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UserStat)
		uint32 bHighScoresCached : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UserStat)
		TArray<FString> ScoreBoardNames;
};

USTRUCT(BlueprintType)
struct FMedalValues {
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		int32 LB_GlodVlaue;
	UPROPERTY(EditAnywhere)
		int32 LB_SilverValue;
	UPROPERTY(EditAnywhere)
		int32 LB_BronzeValue;
	UPROPERTY(EditAnywhere)
		int32 DefaultGold;
	UPROPERTY(EditAnywhere)
		int32 DefaultSilver;
	UPROPERTY(EditAnywhere)
		int32 DefaultBronze;
};

USTRUCT(BlueprintType)
struct FAchievements
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		TArray<FName> AchievementNames;
	UPROPERTY(EditAnywhere)
		TArray<int32> AchievementValues;
	UPROPERTY(EditAnywhere)
		TArray<float> AchievementProgress;
	UPROPERTY(EditAnywhere)
		TArray<float> CachedAchievementProgress;
	UPROPERTY(EditAnywhere)
		uint32 bAchievementsCached : 1;
};

USTRUCT(BlueprintType)
struct FInAppPurchase 
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		FString IAP_Theme;
	UPROPERTY(EditAnywhere)
		uint32 bThemePurchased : 1;
};

USTRUCT(BlueprintType)
struct FSettings 
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		uint32 bPlaySounds : 1;
	UPROPERTY(EditAnywhere)
		uint32 bPlayMusic : 1;
	UPROPERTY(EditAnywhere)
		uint32 bPlayFeedBack : 1;
	UPROPERTY(EditAnywhere)
		uint32 bPlayCameraShake : 1;
	UPROPERTY(EditAnywhere)
		uint32 bIsTouchDevice : 1;
	UPROPERTY(EditAnywhere)
		uint32 bUseTheme : 1;
};