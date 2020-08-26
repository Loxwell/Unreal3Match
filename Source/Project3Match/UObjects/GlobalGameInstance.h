// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
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

//protected:
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
//		TSubclassOf<UUserWidget> UIGameScreen;
private:
	FVector2D StoredCameraViewport;
};
