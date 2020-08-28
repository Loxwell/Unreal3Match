#pragma once

#include "GameFrameWork/PlayerController.h"
#include "Match3PlayerController.generated.h"

UCLASS()
class PROJECT3MATCH_API AMatch3PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = Game)
		int32 CalculateBombPower();
	virtual int32 CalculateBombPower_Implementation();
};
