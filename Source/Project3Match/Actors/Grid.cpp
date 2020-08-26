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
}

void AGrid::OnMoveMade(ETileMoveType::Type MoveType)
{

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

ATile* AGrid::CreateTile(TSubclassOf<ATile> UClassOfTile, class UMaterialInstanceConstant* TileMaterial,
	FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID)
{
	return nullptr;
}

int32 AGrid::SelectTileFromLibrary()
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