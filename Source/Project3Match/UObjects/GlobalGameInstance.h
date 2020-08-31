// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"

#include "../MISC/LevelSaveData.h"

#include "GlobalGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT3MATCH_API UGlobalGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void SetCameraResize(const FVector2D& NewViewport) { StoredCameraViewport = NewViewport; }

	bool FindSaveDataForLevel(UObject* WorldContextObject, FLevelSaveData& SaveGameData);
	void UpdateSave(UObject* WorldContextObject, FLevelSaveData& NewData);
	void SaveGame();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
		TSubclassOf<UUserWidget> UIGameScreen;
private:
	FVector2D StoredCameraViewport;
};
