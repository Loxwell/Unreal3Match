// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Project3MatchGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT3MATCH_API AProject3MatchGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void GameOver();
	
	/// <Summary>
	/// �̵�� C++ å�� �ٽ� �ѹ� �� ��
	/// </Summary>
	UFUNCTION(BlueprintNativeEvent, Category = Game)
		int32 CalculateBombPower();
	virtual int32 CalculateBombPower_Implementation();
	void AddScore(int32 Points);
	int32 GetMaxComboPower();
	int32 GetComboPower();
	/** The game mode understands the concept of combo power. */
	void SetComboPower(int32 NewComboPower);
};
