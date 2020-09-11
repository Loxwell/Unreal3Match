// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Grid.h"
#include "GridInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT3MATCH_API AGridInstance : public AGrid
{
	GENERATED_BODY()
	
public:
	AGridInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), BaseSpeed(500.f) {}
	UFUNCTION(BlueprintCallable)
		void ToggleAllBombs();
	UFUNCTION(BlueprintCallable)
		bool PlayMoveEffect(float AdjustedTileSpeed);
	virtual int32 GetScoreMultiplierForMove(ETileMoveType::Type LastMoveType) override;
	virtual void OnMoveMade_Implementation(ETileMoveType::Type MoveType) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		float BaseSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		float ComboCounter;
};
