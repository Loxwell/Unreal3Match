#include "Grid.h"
#include "Tile.h"
#include "../Project3MatchGameModeBase.h"
#include "Sound/SoundWave.h"
#include "Math/UnrealMathUtility.h"

AGrid::AGrid(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	MinimumRunLength(3)
{
	PrimaryActorTick.bCanEverTick = true;

	TileSize.Set(25, 25);
}

void AGrid::InitGrid()
{
	GameTiles.Empty(GridWidth * GridHeight);
	GameTiles.AddZeroed(GameTiles.Max());
	FVector SpawnLocation;
	for (int32 Column = 0; Column < GridWidth; ++Column)
	{
		for (int32 Row = 0; Row < GridHeight; ++Row)
		{
			int32 TileID = SelectTileIDFromLibrary();
			int32 GridAddress;
			GetGridAddressWithOffset(0, Column, Row, GridAddress);
			SpawnLocation = GetLocationFromGridAddress(GridAddress);

			do {
				TileID = SelectTileIDFromLibrary();
				if ((Column >= MinimumRunLength - 1) || (Row >= MinimumRunLength - 1))
				{
					int32 TestAddress = 0, TileOffset = 0;
					for (int32 Horizontal = 0; Horizontal < 2; ++Horizontal)
					{
						for (TileOffset = 1; TileOffset < MinimumRunLength; ++TileOffset)
						{
							if (!GetGridAddressWithOffset(0, Column - (Horizontal ? TileOffset : 0), Row - (Horizontal ? 0 : TileOffset), TestAddress) ||
								(GetTileFromGridAddress(TestAddress)->GetTileTypeID() != TileID))
								break; // Not in a matching run, or off the edge of the map, so stop checking this axis.
						}

						if(TileOffset == MinimumRunLength)
							break; // We made it through the whole "check for matching run" loop. This tile completes a scoring run. Pick a new tile type and test again.
					}

					if(TileOffset < MinimumRunLength)
						break; // We didn't find a matching run in either direction, so we have a valid tile type.
				}
				else
					break; // This tile is too close to the edge to be worth checking.
			} while (true);

			CreateTile(TileLibrary[TileID].TileClass, TileLibrary[TileID].TileMaterial, SpawnLocation, GridAddress ,TileID);
		}
	}
}

ATile* AGrid::CreateTile(
	TSubclassOf<class ATile> UClassOfTile,
	class UMaterialInstanceConstant* TileMaterial,
	FVector SpawnLocation,
	int32 SpawnGridAddress,
	int32 TileTypeID)
{
	if (UClassOfTile)
	{
		checkSlow(TileLibrary.IsValidIndex(TileTypeID));
		
		UWorld* const World = GetWorld();
		// World 유효성 확인
		if (World)
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.Owner = this;
			SpawnParam.Instigator = AActor::GetInstigator();
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Fixed Axis
			FRotator SpawnRotation(0, 0, 0);

			// Spawn the Tile
			ATile* const NewTile = World->SpawnActor<ATile>(UClassOfTile, SpawnLocation, SpawnRotation, SpawnParam);
			NewTile->GetRenderComponent()->SetMobility(EComponentMobility::Movable);
			NewTile->SetTileTypeID(TileTypeID);
			NewTile->Abilities = TileLibrary[TileTypeID].Abilities;
			NewTile->SetTileMaterial(TileLibrary[TileTypeID].TileMaterial);
			NewTile->SetGridAddress(SpawnGridAddress);
			GameTiles[SpawnGridAddress] = NewTile;

			return NewTile;
		}
	}

	return nullptr;
}

int32 AGrid::SelectTileIDFromLibrary()
{
	float NormailizingFactor = 0;
	for (const FTileType& TileBase : TileLibrary)
	{
		NormailizingFactor += TileBase.Probability;
	}
	return 0;
}

void AGrid::ReturnMatchSound(TArray<USoundWave*>& MatchSounds)
{

}

FVector AGrid::GetLocationFromGridAddress(int32 GridAddress) const
{
	return FVector();
}

FVector AGrid::GetLocationFromGridAddressWithOffset(int32 GridAddress, int32 XOffsetInTiles, int32 YOffsetInTiles) const
{
	return FVector();
}

bool AGrid::GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset, int32& ReturnGridAddress) const
{

	return false;
}

int32 AGrid::GetScoreMultiplierForMove(ETileMoveType::Type LastMoveType)
{
	return 0;
}

ATile* AGrid::GetTileFromGridAddress(int32 GridAddress) const
{
	return nullptr;
}

bool AGrid::AreAddressNeighbors(int32 GridAddress, int32 GridAddressB) const
{
	return false;
}

void AGrid::OnTileFinishedFalling(ATile* Tile, int32 LandingGridAddress)
{

}

void AGrid::OnTileFinishedMatching(ATile* InTile)
{

}

void AGrid::OnSwapDisplayFinished(ATile* InTile)
{

}

void AGrid::RespawnTiles()
{

}

void AGrid::SwapTiles(ATile* From, ATile* To, bool bRepositionTileActors)
{

}

bool AGrid::IsMovingLegal(ATile* Form, ATile* To)
{
	return false;
}

TArray<ATile*> AGrid::GetExploionList(ATile* Tile) const
{
	return TArray<ATile*>();
}

TArray<ATile*> AGrid::FindNeighbors(ATile* StartingTile, bool bMustMatchID, int32 RunLegnth) const
{
	return TArray<ATile*>();
}

TArray<ATile*> AGrid::FindTilesOfType(int32 TileTypeID)
{
	return TArray<ATile*>();
}

void AGrid::ExcuteMatch(const TArray<ATile*>& MatchingTiles)
{

}

void AGrid::OnTileWasSelecting(ATile* NewSelectedTile)
{

}

bool AGrid::IsUnwinnable()
{
	return false;
}

void AGrid::SetLastMove(ETileMoveType::Type MoveType)
{

}

ETileMoveType::Type AGrid::GetLastMove()
{
	return ETileMoveType::Type::MT_NONE;
}