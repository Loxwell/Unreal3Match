
#pragma once

#include "GameFramework/SaveGame.h"
#include "../MISC/LevelSaveData.h"

#include "Match3SaveGame.generated.h"

UCLASS()
class PROJECT3MATCH_API UMatch3SaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	/**
	*	@see UGameplayStatics::CreateSaveGameObject
	*	@see UGameplayStatics::SaveGameToSlot
	*	@see UGameplayStatics::DoesSaveGameExist
	*	@see UGameplayStatics::LoadGameFromSlot
	*	@see UGameplayStatics::DeleteGameInSlot
	*/

	bool LoadCustomInt(const FString& FieldName, int32& Value) const;
	void SaveCustomInt(const FString& FieldName, const int32& Value);
	void ClearCustomInt(const FString& FieldName);

protected:
	UPROPERTY()
		TMap<FString, int32>CustomIntData;

private:
	UPROPERTY()
		TMap<FString, FLevelSaveData> SaveData;

};
