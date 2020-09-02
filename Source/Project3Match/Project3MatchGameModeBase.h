// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MISC/Reward.h"
#include "MISC/LevelSaveData.h"

#include "Project3MatchGameModeBase.generated.h"

#define ATB_UFUNCTION           UFUNCTION(BlueprintCallable, Category = Game)
#define ATB_BP_PURE             UFUNCTION(BlueprintPure, Category = Game)
#define UPROPERTY_BP_READ_ONLY  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
/**
 * 
 */
UCLASS()
class PROJECT3MATCH_API AProject3MatchGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AProject3MatchGameModeBase(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	ATB_UFUNCTION
		void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetUClass);
	ATB_UFUNCTION
		void GameRestart();
	/// <summary>
	/// ���� (����/����) UI Widget ����
	/// </summary>
	ATB_UFUNCTION
		void GameOver();
	ATB_UFUNCTION
		bool IsGameActive() const;
	ATB_UFUNCTION
		void PauseGameTimer(bool bPause);
	ATB_UFUNCTION
		FString GetRemainingTimeAsString();
	ATB_UFUNCTION
		bool GetTimerPaused() const;
	ATB_UFUNCTION
		void UpdateScoresFromLeaderBoard(int32  GoldScore, int32 SilverScore, int32 BronzeSocre);

	ATB_BP_PURE
		int32 GetComboPower();
	ATB_BP_PURE
		int32 GetMaxComboPower();

	UFUNCTION(BlueprintImplementableEvent, Category = Game)
		void AwardPlace(int32 NewPlace, int32 PointsGiven);
	UFUNCTION(BlueprintImplementableEvent, Category = Game)
		void AwardBonus();
	UFUNCTION(BlueprintImplementableEvent, Category = Game)
		void GameWasWon(bool bGameWasWon);

	/// <Summary>
	/// BlueprintNativeEvent == BlueprintImplementableEvent + BlueprintCallable �� ���հ� ������ ����
	/// int32 CalculateBombPower()�� ��� ����Ʈ���� �������ؼ� �����
	/// ��� ����Ʈ���� int32 CalculateBombPower() ������ ���� ȣ�� �� ��� �Լ� �ٵ� ���� ���� �ʾұ� ������
	/// ��� ȣ�� �� �Լ��� CalculateBombPower_Implementation�� ���� �� ���� �ؾ���
	/// ��������� ����� ���� ���� ���� ��� UHT�� ���� ���� �ȴ�(Ȯ�� �ٶ�)
	/// </Summary>
	UFUNCTION(BlueprintNativeEvent, Category = Game)
		int32 CalculateBombPower();
	virtual int32 CalculateBombPower_Implementation();

#pragma region NORMAL_CPP_FUNCS
	void AddScore(int32 Points);
	/** The game mode understands the concept of combo power. */
	void SetComboPower(int32 NewComboPower);
#pragma endregion

#pragma region Getter/Setter
	inline float GetTileMoveSpeed() const { return TileMoveSpeed; }
	ATB_UFUNCTION
		inline int32 GetFinalPlace()const { return FinalPlace; }
	inline void SetFinalPlace(int32 NewFinalPlace) { FinalPlace = NewFinalPlace; }
#pragma endregion

protected:
	UPROPERTY_BP_READ_ONLY
		TSubclassOf<class UUserWidget> StartingWidgetUClass;
	UPROPERTY_BP_READ_ONLY
		TSubclassOf<class UUserWidget> DefeatWidgetUClass;
	UPROPERTY_BP_READ_ONLY
		TSubclassOf<class UUserWidget> VictoryWidgetUClass;
	UPROPERTY()
		class UUserWidget* CurrentWidget;
	UPROPERTY(EditAnywhere, Category = Game)
		float TimeRemaining;

	FTimerHandle GameOverTimer;
	bool bGameWillBeWon;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game, Meta = (AllowPrivateAccess = true))
		TArray<FReward> Rewards;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game, Meta = (AllowPrivateAccess = true))
		FLevelSaveData SaveGameData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game, Meta = (AllowPrivateAccess = true))
		float TileMoveSpeed;
	UPROPERTY()
		int32 FinalPlace;
};
