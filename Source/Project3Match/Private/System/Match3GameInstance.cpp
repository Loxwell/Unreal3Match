// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Match3GameInstance.h"
#include "System/Match3SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealClient.h"
#include "Online.h"
#include "OnlineSubsystem.h"

#define GetLevelName(exp) (UGameplayStatics::GetCurrentLevelName((exp), true))

UMatch3GameInstance::UMatch3GameInstance()
{
	DefaultSaveGameSlot = TEXT("_Match3Game");
}

bool UMatch3GameInstance::FindSaveDataForLevel(UObject* WorldContextObject, FLevelSaveData& OutSaveData)
{
	if (FLevelSaveData* FoundData = InstanceGameData->Find(GetLevelName(WorldContextObject)))
	{
		OutSaveData = *FoundData;
		return true;
	}
	return false;
}

void UMatch3GameInstance::UpdateSave(UObject* WorldContextObject, FLevelSaveData& NewData)
{
	return;
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(WorldContextObject, true);
	FLevelSaveData* CurrentData = InstanceGameData->Find(LevelName);
	if (CurrentData)
	{
		*CurrentData = NewData;
		UpdateUIAfterSave();
	}
}

void UMatch3GameInstance::SaveGame()
{
	UGameplayStatics::SaveGameToSlot(InstanceGameData, GetSaveSlotName(), 0);
}

bool UMatch3GameInstance::LoadCustomInt(const FString& FieldName, int32& Value)
{
	check(InstanceGameData);
	return InstanceGameData->LoadCustomInt(FieldName, Value);
}

void UMatch3GameInstance::SaveCustomInt(const FString& FieldName, const int32& Value)
{
	check(InstanceGameData);
	InstanceGameData->SaveCustomInt(FieldName, Value);
}

void UMatch3GameInstance::ClearCustomInt(const FString& FieldName)
{
	check(InstanceGameData);
	InstanceGameData->ClearCustomInt(FieldName);
}

void UMatch3GameInstance::Init()
{
	InitSaveGameSlot();

	LoginChangedHandle = FCoreDelegates::OnUserLoginChangedEvent.AddUObject(this, &UMatch3GameInstance::OnLoginChanged);
	EnteringForegroundHandle = FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddUObject(this, &UMatch3GameInstance::OnEnteringForeground);
	EnteringBakcgroundHandle = FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddUObject(this, &UMatch3GameInstance::OnEnteringBackground);
	ViewportHandle = FViewport::ViewportResizedEvent.AddUObject(this, &UMatch3GameInstance::OnViewportResize_Internal);

	IOnlinePurchasePtr PurchaseInterface = Online::GetPurchaseInterface();
	if (PurchaseInterface.IsValid())
		UnexpectedPurchaseHandle = PurchaseInterface->AddOnUnexpectedPurchaseReceiptDelegate_Handle(FOnUnexpectedPurchaseReceiptDelegate::CreateUObject(this, &UMatch3GameInstance::OnUnexpectedPurchase_Internal));
	
	Super::Init();
}

void UMatch3GameInstance::Shutdown()
{
	FCoreDelegates::OnUserLoginChangedEvent.Remove(LoginChangedHandle);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.Remove(EnteringForegroundHandle);
	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.Remove(EnteringBakcgroundHandle);
	FViewport::ViewportResizedEvent.Remove(ViewportHandle);

	Super::Shutdown();
}

void UMatch3GameInstance::InitSaveGameSlot()
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

FString UMatch3GameInstance::GetSaveSlotName() const
{
	return SaveGamePrefix + DefaultSaveGameSlot;
}

void UMatch3GameInstance::RegisterOnlineID(const FString& NewOnlineID)
{
	SaveGamePrefix = NewOnlineID;
	InitSaveGameSlot();
}

void UMatch3GameInstance::OnViewportResize_Internal(FViewport* Viewport, uint32 ID)
{
	OnViewportResize();
}

void UMatch3GameInstance::OnUnexpectedPurchase_Internal(const FUniqueNetId& UserId)
{
	OnUnexpectedPurchase();
}
