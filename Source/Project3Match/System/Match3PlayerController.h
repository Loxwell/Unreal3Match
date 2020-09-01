#pragma once

#include "GameFrameWork/PlayerController.h"
#include "Match3PlayerController.generated.h"

UCLASS()
class PROJECT3MATCH_API AMatch3PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		inline int32 GetScore() const { return 0; }
	UFUNCTION(BlueprintCallable)
		void AddScore(const int32& Points) { }

	UFUNCTION(BlueprintNativeEvent, Category = Game)
		int32 CalculateBombPower();
	virtual int32 CalculateBombPower_Implementation();

	int32 GetMaxComboPower() const { return 0; }
	int32 GetComboPower() const { return 0; }
	void SetComboPower(const int32& NewComboPower) {}
};
