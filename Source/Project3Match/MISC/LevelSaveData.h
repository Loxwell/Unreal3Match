#pragma once

#include "UObject/ObjectMacros.h"
#include "LevelSaveData.generated.h"

#define BP_READ_ONLY UPROPERTY(EditAnywhere, BlueprintReadOnly)

USTRUCT(BlueprintType)
struct FLevelSaveData
{
	GENERATED_USTRUCT_BODY()
public:
	BP_READ_ONLY
		int32 GoldScore;
	BP_READ_ONLY
		int32 SilverScore;
	BP_READ_ONLY
		int32 BronzeScore;
	BP_READ_ONLY
		int32 TopScore;
};