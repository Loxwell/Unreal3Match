// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "../MISC/LevelSaveData.h"
#include "GlobalGameInstance.generated.h"

#define ATB_UFUNCTION UFUNCTION(BlueprintCallable, Category = "Saved Game")

/**
 * 
 */
UCLASS()
class PROJECT3MATCH_API UGlobalGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UGlobalGameInstance();

	// 게임 시작 시 호출
	virtual void Init() override;

	// 프로그램이 종료 될 때 호출
	virtual void Shutdown() override;

	ATB_UFUNCTION
		void SaveGame();
	ATB_UFUNCTION
		bool LoadCustomInt(const FString& FieldName, int32& Value);
	ATB_UFUNCTION
		void SaveCustomInt(const FString& FieldName, const int32& Value);
	ATB_UFUNCTION
		void SaveCustomInt(const FString& FieldName, const int32&& Value);
	ATB_UFUNCTION
		void ClearCustomInt(const FString& FieldName);

	UFUNCTION(BlueprintCallable, Category = Online)
		void RegisterOnlineID(const FString& NewOnlineID);

#pragma region BLUEPRINT_IMPEMENT_EVENT
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateUIAfterSave();
	UFUNCTION(BlueprintImplementableEvent, Category = Online)
		void OnLoginChanged(bool bLoggingIn, const int32& UserID, const int32& UserIndex);
	UFUNCTION(BlueprintImplementableEvent, Category = Online)
		void OnEnteringForeground();
	UFUNCTION(BlueprintImplementableEvent, Category = Online)
		void OnEnteringBackground();
	UFUNCTION(BlueprintImplementableEvent, Category = Online)
		void OnUnexpectedPurchase();
	UFUNCTION(BlueprintImplementableEvent, Category = UI)
		void OnViewportResize();
#pragma endregion

	void OnUnexpectedPurchase_Internal(const FUniqueNetId& UserId);
	void OnViewportResize_Internal(FViewport* const Viewport, const uint32& ID);

	void InitSaveGameSlot();
	
	bool FindSaveDataForLevel(UObject* WorldContextObject, FLevelSaveData& SaveGameData);

	void SetCameraResize(const FVector2D& NewViewport) { StoredCameraViewport = NewViewport; }
	void UpdateSave(UObject* WorldContextObject, FLevelSaveData& NewData);
	
protected:
	FString GetSaveSlotName() const;
	FString SaveGamePrefix();
	FString DefaultSaveGameSlot();

protected:
	// 상속 받은 클래스에서 정의 할 것
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
		TSubclassOf<UUserWidget> UIGameScreen;
	FVector2D StoredCameraViewport;

private:
	/*UPROPERTY()
		class UMatch3SaveGame* InstanceGameData;*/
	FDelegateHandle LoginChangedHandle;
	FDelegateHandle EnteringForgroundHandle;
	FDelegateHandle EnteringBakcgroundHandle;
	FDelegateHandle ViewportHandle;
	FDelegateHandle UnexpectedPurchaseHandle;
};
