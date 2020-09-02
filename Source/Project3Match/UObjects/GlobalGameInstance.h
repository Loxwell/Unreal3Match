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
	UGlobalGameInstance();

	// ���� ���� �� ȣ��
	virtual void Init() override;

	// ���α׷��� ���� �� �� ȣ��
	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable, Category = "Saved Game")
		void SaveGame();
	UFUNCTION(BlueprintCallable, Category = "Saved Game")
		bool LoadCustomInt(const FString& FieldName, int32& Value);
	UFUNCTION(BlueprintCallable, Category = "Saved Game")
		void SaveCustomInt(const FString& FieldName, const int32& Value);
	UFUNCTION(BlueprintCallable, Category = "Saved Game")
		void ClearCustomInt(const FString& FieldName);

	UFUNCTION(BlueprintCallable, Category = Online)
		void RegisterOnlineID(const FString& NewOnlineID);

#pragma region BLUEPRINT_IMPEMENT_EVENT
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateUIAfterSave();
	UFUNCTION(BlueprintImplementableEvent, Category = Online)
		void OnLoginChanged(bool bLoggingIn, int32 UserID, int32 UserIndex);
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
	void OnViewportResize_Internal(FViewport* Viewport, uint32 ID);

	void InitSaveGameSlot();
	
	bool FindSaveDataForLevel(UObject* WorldContextObject, FLevelSaveData& OutSaveData);

	void SetCameraResize(const FVector2D& NewViewport) { StoredCameraViewport = NewViewport; }
	void UpdateSave(UObject* WorldContextObject, FLevelSaveData& NewData);
	
protected:
	FString GetSaveSlotName() const;
	FString SaveGamePrefix;
	FString DefaultSaveGameSlot;

protected:
	// ��� ���� Ŭ�������� ���� �� ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
		TSubclassOf<UUserWidget> UIGameScreen;
	FVector2D StoredCameraViewport;

private:
	/// <summary>
	/// UGameplayStatics�� ���� Serialize�� �ϱ� ���� USaveGame ��ü
	/// </summary>
	UPROPERTY()
		class UMatch3SaveGame* InstanceGameData;
	FDelegateHandle LoginChangedHandle;
	FDelegateHandle EnteringForegroundHandle;
	FDelegateHandle EnteringBakcgroundHandle;
	FDelegateHandle ViewportHandle;
	FDelegateHandle UnexpectedPurchaseHandle;
};
