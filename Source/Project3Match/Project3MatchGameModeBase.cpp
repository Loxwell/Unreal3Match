// Fill out your copyright notice in the Description page of Project Settings.


#include "Project3MatchGameModeBase.h"

#include "GameFrameWork/Pawn.h"
#include "Kismet/GameplayStatics.h"

#include "System/Match3PlayerController.h"
#include "UObjects/GlobalGameInstance.h"
#include "MISC/Match3BPFunctionLibrary.h"

using A3GameMode = AProject3MatchGameModeBase;

A3GameMode::AProject3MatchGameModeBase(const FObjectInitializer& ObjectInitializer)
	: TimeRemaining(5), bGameWillBeWon(false), TileMoveSpeed(50), FinalPlace(0)
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = APawn::StaticClass();
	PlayerControllerClass = AMatch3PlayerController::StaticClass();
}

void A3GameMode::BeginPlay()
{
	Super::BeginPlay();

	bGameWillBeWon = false;
	
	// 시작 UI 생성
	ChangeMenuWidget(StartingWidgetUClass);
	
	// Game Over timer 설정
	// TimeRemaining 시간 설정 또는 추후 다른 종료 조건 설정 할 것
	GetWorldTimerManager().SetTimer(GameOverTimer, this, &AProject3MatchGameModeBase::GameOver, TimeRemaining, false);
	if (UGlobalGameInstance* GameInstance = Cast<UGlobalGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		// Save or Update Data
		if (!GameInstance->FindSaveDataForLevel(this, SaveGameData))
			GameInstance->UpdateSave(this, SaveGameData);
	}
}

void A3GameMode::GameRestart()
{
	ChangeMenuWidget(nullptr);
	FName LevelName(*UGameplayStatics::GetCurrentLevelName(this, true));
	UGameplayStatics::OpenLevel(this, LevelName);
}

void A3GameMode::GameOver()
{
	GetWorldTimerManager().ClearTimer(GameOverTimer);

	if (bGameWillBeWon)
	{
		// 최고 점수 처리 코드
		if (AMatch3PlayerController* PC = Cast<AMatch3PlayerController>(UMatch3BPFunctionLibrary::GetLocalPlayerController(this)))
			SaveGameData.TopScore = FMath::Max(PC->GetScore(), SaveGameData.TopScore);

		if (UGlobalGameInstance* GameInstance = Cast<UGlobalGameInstance>(UGameplayStatics::GetGameInstance(this)))
		{
			GameInstance->UpdateSave(this, SaveGameData);
			GameInstance->SaveGame();
		}

		ChangeMenuWidget(bGameWillBeWon ? VictoryWidgetUClass : DefeatWidgetUClass);
		GameWasWon(bGameWillBeWon);
	}
}

bool A3GameMode::IsGameActive() const
{
	FTimerManager& WorldTimerManager = GetWorldTimerManager();
	return WorldTimerManager.IsTimerActive(GameOverTimer) || WorldTimerManager.IsTimerPaused(GameOverTimer);
}

void A3GameMode::PauseGameTimer(bool bPause)
{
	if (bPause)
		GetWorldTimerManager().PauseTimer(GameOverTimer);
	else
		GetWorldTimerManager().UnPauseTimer(GameOverTimer);
}

FString A3GameMode::GetRemainingTimeAsString()
{
	int32 OutInt = FMath::CeilToInt(GetWorldTimerManager().GetTimerRemaining(GameOverTimer));
	return FString::Printf(TEXT("%03i"), FMath::Max(0, OutInt));
}

bool A3GameMode::GetTimerPaused() const 
{
	return GetWorldTimerManager().IsTimerPaused(GameOverTimer);
}

void A3GameMode::AddScore(int32 Points)
{
	if (AMatch3PlayerController* PC = Cast<AMatch3PlayerController>(UMatch3BPFunctionLibrary::GetLocalPlayerController(this)))
	{
		int32 OldScore = PC->GetScore();
		PC->AddScore(Points);
		int32 NewScore = PC->GetScore();
		if (NewScore >= SaveGameData.GoldScore)
		{

		}

	}
}

void A3GameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetUClass)
{

}

void A3GameMode::UpdateScoresFromLeaderBoard(int32 GoldScore, int32 SilverScore, int32 BronzeScore)
{

}

int32 A3GameMode::GetComboPower()
{
	return 0;
}

int32 A3GameMode::GetMaxComboPower()
{
	return 0;
}

int32 A3GameMode::CalculateBombPower_Implementation()
{
	if (AMatch3PlayerController* PC = 
		Cast<AMatch3PlayerController>(UMatch3BPFunctionLibrary::GetLocalPlayerController(this)))
		return PC->CalculateBombPower();
	return 0;
}

void A3GameMode::SetComboPower(int32 NewComboPower)
{

}
