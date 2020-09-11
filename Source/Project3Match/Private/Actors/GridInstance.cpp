// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GridInstance.h"
#include "Actors/Tile.h"
#include "Kismet/GameplayStatics.h"
#include "../Project3MatchGameModeBase.h"

#define CHECK_IF_COMBO_POWER_IS_MAX ( \
	Cast<AProject3MatchGameModeBase>(UGameplayStatics::GetGameMode(this))->GetComboPower() == \
	Cast<AProject3MatchGameModeBase>(UGameplayStatics::GetGameMode(this))->GetMaxComboPower())

bool CheckIfComboPowerIsMax(UObject* WorldContext)
{
	static AProject3MatchGameModeBase* GameMode = Cast<AProject3MatchGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
	return GameMode->GetComboPower() == GameMode->GetMaxComboPower();
}

int32  AGridInstance::GetScoreMultiplierForMove(ETileMoveType::Type LastMoveType)
{
	int32 Score = 0;

	switch (LastMoveType)
	{
	case ETileMoveType::MT_STANDARD:
		Score = 100;
		break;
	case ETileMoveType::MT_MORE_TILES:
		Score = 150;
		break;
	case ETileMoveType::MT_COMBO:
		Score = 200;
		break;
	case ETileMoveType::MT_ALL_THE_BOMBS:
		Score = 50;
		break;
	}

	return Score;
}

void AGridInstance::ToggleAllBombs()
{
	if (CHECK_IF_COMBO_POWER_IS_MAX)
	{
		for (TArray<ATile*>::ElementType Tile : GameTiles)
		{
			if (::IsValid(Tile) && Tile->GetBPReference())
				Tile->ProcessEvent(Tile->GetBPReference()->FindFunction(FName("ToggleBombFlashing")), nullptr);
		}
	}
}

bool AGridInstance::PlayMoveEffect(float AdjustedTileSpeed)
{
	AProject3MatchGameModeBase* GameMode = Cast<AProject3MatchGameModeBase>(UGameplayStatics::GetGameMode(this));
	float MaxComboEffect = 10;
	GameMode->SetTileMoveSpeed(FMath::Clamp(AdjustedTileSpeed, 0.f, BaseSpeed * MaxComboEffect));

	return FMath::Clamp(AdjustedTileSpeed, 0.f, BaseSpeed * MaxComboEffect) >= BaseSpeed * MaxComboEffect;
}

void AGridInstance::OnMoveMade_Implementation(ETileMoveType::Type MoveType)
{
	using namespace ETileMoveType;
	switch (MoveType)
	{
	case MT_BOMB:
	case MT_ALL_THE_BOMBS:
	case MT_FAILURE:
	case MT_STANDARD:
	case MT_MORE_TILES:
		ComboCounter = 1;
		PlayMoveEffect(BaseSpeed);
		break;
	case MT_COMBO:
		ComboCounter *= 1.5f;
		PlayMoveEffect(BaseSpeed);
		ToggleAllBombs();
		break;
	}
}