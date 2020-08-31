#pragma once

#include "UObject/ObjectMacros.h"
#include "Reward.generated.h"

USTRUCT(BlueprintType)
struct FReward 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ScoreInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TimeAwarded;
};