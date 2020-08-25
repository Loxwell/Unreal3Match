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
	inline void SetCameraResize(const FVector2D& NewViewport) { StoredCameraViewport = NewViewport; }
	
private:
	/*UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
		*/
	UPROPERTY()
		TSubclassOf<UUserWidget> UIGameScreen;
	FVector2D StoredCameraViewport;
};
