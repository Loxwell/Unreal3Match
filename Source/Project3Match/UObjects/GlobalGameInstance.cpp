// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalGameInstance.h"

UGlobalGameInstance::UGlobalGameInstance()
{

}

void UGlobalGameInstance::OnUnexpectedPurchase_Internal(const FUniqueNetId& UserId)
{
}

void UGlobalGameInstance::OnViewportResize_Internal(FViewport* const Viewport, const uint32& ID)
{
}

void UGlobalGameInstance::InitSaveGameSlot()
{
}

bool UGlobalGameInstance::FindSaveDataForLevel(UObject* WorldContextObject, FLevelSaveData& SaveGameData)
{
	return false;
}

void UGlobalGameInstance::UpdateSave(UObject* WorldContextObject, FLevelSaveData& NewData)
{

}

FString UGlobalGameInstance::GetSaveSlotName() const
{
	return FString();
}

FString UGlobalGameInstance::SaveGamePrefix()
{
	return FString();
}

FString UGlobalGameInstance::DefaultSaveGameSlot()
{
	return FString();
}

void UGlobalGameInstance::Init()
{
}

void UGlobalGameInstance::Shutdown()
{
}

void UGlobalGameInstance::SaveGame()
{

}

bool UGlobalGameInstance::LoadCustomInt(const FString& FieldName, int32& Value)
{
	return false;
}

void UGlobalGameInstance::SaveCustomInt(const FString& FieldName, const int32& Value)
{
}

void UGlobalGameInstance::SaveCustomInt(const FString& FieldName, const int32&& Value)
{
}

void UGlobalGameInstance::ClearCustomInt(const FString& FieldName)
{
}

void UGlobalGameInstance::RegisterOnlineID(const FString& NewOnlineID)
{
}

void SaveGame()
{
}
