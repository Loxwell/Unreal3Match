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
	ADerivedMatch3GameMode(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void Tick(float DeltaSeconds) override;
	
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
	UPROPERTY()
		class UUserWidget* MainUI;
	UPROPERTY()
		class UTextBlock* TextScore;

	HelperClasses::FDoOnceFlag DoOnce;
	FText TimeAsText;
	FTimerHandle GameTimeHandler;

	float ElapsedTime;
};
