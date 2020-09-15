// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Project3MatchGameModeBase.h"
#include "MISC/HelperClasses.h"
#include "DerivedMatch3GameMode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateNoneParam);
/**
 * 
 */
UCLASS()
class PROJECT3MATCH_API ADerivedMatch3GameMode : public AProject3MatchGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

private:
	UFUNCTION()
		void GameTimer();
	bool TimeAddedCheck();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Game)
		void MatchBombsAchievement();
	UFUNCTION(BlueprintImplementableEvent, Category = Game)
		void MatchBlockAchievement();

	void SpawnScoreDisplay(FVector WorldLocaton);

public:
	UPROPERTY(BlueprintAssignable, Category = "GameMode|Delegate")
		FDelegateNoneParam UpdateTimeDisplay;
	UPROPERTY(BlueprintAssignable, Category = "GameMode|Delegate")
		FDelegateNoneParam UpdateScoreDisplay;
	UPROPERTY(BlueprintAssignable, Category = "GameMode|Delegate")
		FDelegateNoneParam GameStarted;

private:
	UPROPERTY()
		TSubclassOf<class UUserWidget> UIClass;
	class UUserWidget* MainUI;
	HelperClasses::FDoOnceFlag DoOnce;
	FText TimeAsText;
	FTimerHandle GameTimeHandler;
};
