#include "Match3SaveGame.h"

bool UMatch3SaveGame::LoadCustomInt(const FString& FieldName, int32& Value) const
{
	if (const int32* ValuePointer = CustomIntData.Find(FieldName))
	{
		Value = *ValuePointer;
		return true;
	}
	
	return false;
}

void UMatch3SaveGame::SaveCustomInt(const FString& FieldName, const int32& Value)
{
	CustomIntData.FindOrAdd(FieldName, Value);
}

void UMatch3SaveGame::ClearCustomInt(const FString& FiedlName)
{
	CustomIntData.Remove(FiedlName);
}