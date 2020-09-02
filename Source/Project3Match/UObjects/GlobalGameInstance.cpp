// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../System/Match3SaveGame.h"

#include "UnrealClient.h"
#include "Online.h"
#include "OnlineSubsystem.h"

#define GetLevelName(exp) (UGameplayStatics::GetCurrentLevelName((exp), true))

UGlobalGameInstance::UGlobalGameInstance()
{
	DefaultSaveGameSlot = TEXT("_Match3Game");
}

bool UGlobalGameInstance::FindSaveDataForLevel(UObject* WorldContextObject, FLevelSaveData& OutSaveData)
{
	if (FLevelSaveData* FoundData = InstanceGameData->Find(GetLevelName(WorldContextObject)))
	{
		OutSaveData = *FoundData;
		return true;
	}
	return false;
}

void UGlobalGameInstance::UpdateSave(UObject* WorldContextObject, FLevelSaveData& NewData)
{
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(WorldContextObject, true);
	FLevelSaveData* CurrentData = InstanceGameData->Find(LevelName);
	*CurrentData = NewData;
	UpdateUIAfterSave();
}

void UGlobalGameInstance::SaveGame()
{
	UGameplayStatics::SaveGameToSlot(InstanceGameData, GetSaveSlotName(), 0);
}

bool UGlobalGameInstance::LoadCustomInt(const FString& FieldName, int32& Value)
{
	check(InstanceGameData);
	return InstanceGameData->LoadCustomInt(FieldName, Value);
}

void UGlobalGameInstance::SaveCustomInt(const FString& FieldName, const int32& Value)
{
	check(InstanceGameData);
	InstanceGameData->SaveCustomInt(FieldName, Value);
}

void UGlobalGameInstance::ClearCustomInt(const FString& FieldName)
{
	check(InstanceGameData);
	InstanceGameData->ClearCustomInt(FieldName);
}

void UGlobalGameInstance::Init()
{
	InitSaveGameSlot();

	LoginChangedHandle = FCoreDelegates::OnUserLoginChangedEvent.AddUObject(this, &UGlobalGameInstance::OnLoginChanged);
	EnteringForegroundHandle = FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddUObject(this, &UGlobalGameInstance::OnEnteringForeground);
	EnteringBakcgroundHandle = FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddUObject(this, &UGlobalGameInstance::OnEnteringBackground);
	ViewportHandle = FViewport::ViewportResizedEvent.AddUObject(this, &UGlobalGameInstance::OnViewportResize_Internal);

	IOnlinePurchasePtr PurchaseInterface = Online::GetPurchaseInterface();
	if (PurchaseInterface.IsValid())
		UnexpectedPurchaseHandle = PurchaseInterface->AddOnUnexpectedPurchaseReceiptDelegate_Handle(FOnUnexpectedPurchaseReceiptDelegate::CreateUObject(this, &UGlobalGameInstance::OnUnexpectedPurchase_Internal));
	
	Super::Init();
}

void UGlobalGameInstance::Shutdown()
{
	FCoreDelegates::OnUserLoginChangedEvent.Remove(LoginChangedHandle);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.Remove(EnteringForegroundHandle);
	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.Remove(EnteringBakcgroundHandle);
	FViewport::ViewportResizedEvent.Remove(ViewportHandle);

	Super::Shutdown();
}

void UGlobalGameInstance::InitSaveGameSlot()
{
	const FString SaveSlotName = GetSaveSlotName();
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		// Clear default save file, if it exists.
		if (UGameplayStatics::DoesSaveGameExist(DefaultSaveGameSlot, 0))
			UGameplayStatics::DeleteGameInSlot(DefaultSaveGameSlot, 0);

		// If we have no save object, create one.
		if (InstanceGameData == nullptr)
			// We're either not logged in with an Online ID, 
			// or we have no save data to transfer over (usually, this indicates program startup).
			InstanceGameData = Cast<UMatch3SaveGame>(UGameplayStatics::CreateSaveGameObject(UMatch3SaveGame::StaticClass()));
		UGameplayStatics::SaveGameToSlot(InstanceGameData, SaveSlotName, 0);
	}
	else
		InstanceGameData = Cast<UMatch3SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	check(InstanceGameData);
}

FString UGlobalGameInstance::GetSaveSlotName() const
{
	return SaveGamePrefix + DefaultSaveGameSlot;
}

void UGlobalGameInstance::RegisterOnlineID(const FString& NewOnlineID)
{
	SaveGamePrefix = NewOnlineID;
	InitSaveGameSlot();
}

void UGlobalGameInstance::OnViewportResize_Internal(FViewport* Viewport, uint32 ID)
{
	OnViewportResize();
}

void UGlobalGameInstance::OnUnexpectedPurchase_Internal(const FUniqueNetId& UserId)
{
	OnUnexpectedPurchase();
}
