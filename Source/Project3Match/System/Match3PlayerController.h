#pragma once

#include "GameFrameWork/PlayerController.h"
#include "Match3PlayerController.generated.h"

UCLASS()
class PROJECT3MATCH_API AMatch3PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMatch3PlayerController(const FObjectInitializer& ObjectInitializer);

#pragma region BLUEPRINT_FUNCTION
	/** Add points. If points are negative or we force immediate update, 
	the score will display instantly instead of counting up. */
	UFUNCTION(BlueprintCallable, Category = Game)
		void AddScore(int32 Points, bool bForceImmediateUpdate = false);
	/** Get the actual score (not the score that is displayed) */
	UFUNCTION(BlueprintCallable, Category = Game)
		inline int32 GetScore() const { return Score; }
	/** Get the score that is currently displayed (not the actual score) */
	UFUNCTION(BlueprintCallable, Category = Game)
		inline int32 GetDisplayedScore() const { return (int32)DisplayedScore; };
#pragma endregion

	/** Override in BPs to power up bombs. */
	UFUNCTION(BlueprintNativeEvent, Category = Game)
		int32 CalculateBombPower();
	virtual int32 CalculateBombPower_Implementation();

	int32 GetMaxComboPower() const { return MaxComboPower; }
	int32 GetComboPower() const { return ComboPower; }
	void SetComboPower(const int32& NewComboPower) { ComboPower = NewComboPower; }

protected:
	/** Periodic function to manage score updates */
	void TickScoreDisplay();

protected:
	/** Current actual score, though not the display value. */
	UPROPERTY()
		int32 Score;
	/** Score that is displayed on screen (as an integer). */
	UPROPERTY()
		float DisplayedScore;
	/** Rate at which displayed score climbs to reach actual score. 
	Currently does not go faster with bigger scores. */
	UPROPERTY(EditAnywhere)
		float ScoreChangeRate;

	FTimerHandle TickScoreDisplayHandle;

private:
	/** Current combo power. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game, Meta = (AllowPrivateAccess = true))
		int32 ComboPower;
	/** Maximum combo power for this player, 
	can be changed based on avatar. 
	TODO: Set this from the avatar class. (version 2.0) 
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game, Meta = (AllowPrivateAccess = true))
		int32 MaxComboPower;
};
