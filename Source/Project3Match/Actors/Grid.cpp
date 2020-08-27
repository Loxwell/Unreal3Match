#include "Grid.h"
#include "Tile.h"
#include "../Project3MatchGameModeBase.h"
#include "../MISC/EnumLibrary.h"

#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
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
		// World ��ȿ�� Ȯ��
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
		NormailizingFactor += TileBase.Probability;

	float TestNumber = FMath::FRandRange(0, NormailizingFactor);
	float CompareTo = 0;

	for (int32 ArrayChecked = 0, TileNum = TileLibrary.Num(); ArrayChecked != TileNum; ++ArrayChecked)
	{
		CompareTo += TileLibrary[ArrayChecked].Probability;
		if (TestNumber <= CompareTo)
			return ArrayChecked;
	}
	return 0;
}

ATile* AGrid::GetTileFromGridAddress(int32 GridAddress) const
{
	if (GameTiles.IsValidIndex(GridAddress))
		return GameTiles[GridAddress];
	return nullptr;
}

FVector AGrid::GetLocationFromGridAddress(int32 GridAddress) const
{
	FVector Center = GetActorLocation();
	// ���ϴ�
	FVector OutLocation = FVector(-(GridWidth * 0.5f) * TileSize.X + (TileSize.X * 0.5f), 0.f, -(GridHeight * 0.5f) * TileSize.Y + (TileSize.Y * 0.5f));
	check(GridWidth > 0);
	OutLocation.X += TileSize.X * (float)(GridAddress % GridWidth);
	OutLocation.Y += TileSize.Y * (float)(GridAddress / GridWidth);
	OutLocation += Center;
	return OutLocation;
}

FVector AGrid::GetLocationFromGridAddressWithOffset(int32 GridAddress, int32 XOffsetInTiles, int32 YOffsetInTiles) const
{
	FVector OutLocation = GetLocationFromGridAddress(GridAddress);
	OutLocation.X += TileSize.X * (float)(XOffsetInTiles);
	OutLocation.Y += TileSize.Y * (float)(YOffsetInTiles);
	return OutLocation;
}

/**
* @param InitialGridAddress : �˻� �� Tile ���� Index
*/
bool AGrid::GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset, int32& ReturnGridAddress) const
{
	int32 NewAxisValue = 0;
	// ���� �� ��ġ�� �ʱ�ȭ
	// ȣ�� �� ������ �ش� �� ���� Ȯ��
	ReturnGridAddress = -1;

	// Check for going off the map in the X direction.
	// X �������� ������ ���� ������ Ȯ��
	check(GridWidth > 0);

	NewAxisValue = (InitialGridAddress % GridWidth) + XOffset;
	// 0��° ���� ���� ���� - 1 ���� ��
	if(NewAxisValue != FMath::Clamp(NewAxisValue, 0, (GridWidth - 1)))
		return false;
	
	NewAxisValue = (InitialGridAddress / GridWidth) + YOffset;
	// 0��° ���� ���� ���� - 1 ���� ��
	if (NewAxisValue != FMath::Clamp(NewAxisValue, 0, (GridHeight - 1)))
		return false;

	// GameTiles�� 1�� �迭 ���� ��� ���
	// ���� Index�� ���� ���� XOffset ��ŭ ��� �̵�
	// XOffset ��ŭ �̵� �� ��ġ ���� ���� �������� YOffset * GridWidth ��ŭ Shift
	ReturnGridAddress = (InitialGridAddress + XOffset + (YOffset * GridWidth));
	check(ReturnGridAddress >= 0);
	check(ReturnGridAddress < (GridWidth * GridHeight));

	return true;
}

bool AGrid::AreAddressesNeighbors(int32 GridAddressA, int32 GridAddressB) const
{
	if ((FMath::Min(GridAddressA, GridAddressB) >= 0)
		&& (FMath::Max(GridAddressA, GridAddressB) < (GridWidth * GridHeight)))
	{
		int32 GridAddressOffset = FMath::Abs(GridAddressA - GridAddressB);

		// �� Index�� ���� �Ǵ� ���� �� (0 - Width or Width - 0) �� ���
		return ((GridAddressOffset == 1) || (GridAddressOffset == GridWidth));
	}
	return false;
}

void AGrid::OnTileFinishedFalling(ATile* Tile, int32 LandingGridAddress)
{
	int32 ReturnGridAddress = 0;

	// Remove the tile from its original position 
	// if it's still there (hasn't been replaced by another falling tile).
	if (GetGridAddressWithOffset(Tile->GetGridAddress(), 0, 0, ReturnGridAddress))
	{
		// �ش� Index���� �ٸ� Tile�� ��ġ �Ǿ��µ� ������ ���� ���ڰ� Tile�� ����ų ���
		if (GameTiles[ReturnGridAddress] == Tile)
			GameTiles[ReturnGridAddress] = nullptr;
	}

	// Validate new grid address and replace whatever is there.
	// ��ȿ�� Tile Index �� �ش� Tile ��� ������ Tile�� ��ü
	if (GetGridAddressWithOffset(LandingGridAddress, 0, 0, ReturnGridAddress))
	{
		GameTiles[ReturnGridAddress] = Tile;
		Tile->SetGridAddress(ReturnGridAddress);
		using namespace ETileState;
		Tile->SetTileState(ETS_NORMAL);
	}

	// This tile is no longer falling, remove it from the list.
	FallingTiles.RemoveSingleSwap(Tile);
	TilesToCheck.Add(Tile);

	// Done with all falling tiles. Spawn new ones at the top of each column in the appropriate quantity.
	// Ÿ���� ���� �̵��� �Ϸ� �� ���, ��� ���� ��� �ִ� Column�� ���ο� Tile ����
	if (FallingTiles.Num() == 0)
		RespawnTiles();
}

void AGrid::OnTileFinishedMatching(ATile* InTile)
{
	if (InTile)
	{
		TilesBeingDestroyed.RemoveSwap(InTile);
		InTile->AActor::Destroy();
	}

	if (TilesBeingDestroyed.Num() == 0)
	{
		for (ATile* Tile : FallingTiles)
			Tile->StartFalling();
		if (FallingTiles.Num() == 0)
			RespawnTiles();
	}
}

void AGrid::RespawnTiles()
{
	for (int32 x = 0; x < GridWidth; ++x)
	{
		int32 BaseAddress = 0, TestAddress = 0;
		if (GetGridAddressWithOffset(0, x, GridHeight - 1, BaseAddress))
		{
			int32 YDepth = 0;
			// BaseAddress �� TestAddress ��
			for (YDepth = 0; GetGridAddressWithOffset(BaseAddress, 0, -YDepth, TestAddress)
				&& (!GetTileFromGridAddress(TestAddress)); ++YDepth)
			{ /* This loop finds the lowest Y value, but does nothing with it.*/ }
			
			for (int32 y = YDepth - 1; y >= 0; --y)
			{
				int32 NewTileTypeID = SelectTileIDFromLibrary();
				GetGridAddressWithOffset(BaseAddress, 0, -y, TestAddress);

				/// <summary>
				/// Move our tile up visually so it has room to fall, but don't change its grid address. 
				/// The new grid address would be off-grid and invalid anyway.
				/// </summary>
				if (ATile* NewTile = CreateTile(TileLibrary[NewTileTypeID].TileClass, TileLibrary[NewTileTypeID].TileMaterial,
					GetLocationFromGridAddressWithOffset(TestAddress, 0, (YDepth + 1)), TestAddress, NewTileTypeID))
				{
					TilesToCheck.Add(NewTile);
					NewTile->SetTileState(ETileState::ETS_FALLING);
					check(!FallingTiles.Contains(NewTile));
					FallingTiles.Add(NewTile);
				}

			}
		}
		else
			check(false);

		if (FallingTiles.Num() > 0)
		{
			/// <summary>
			/// Any falling tiles that exist at this point are new ones, 
			/// and are falling from physical locations (off-grid) to their correct locations.
			/// </summary>
			for (ATile* Tile : FallingTiles)
				Tile->StartFalling();
		}

		TArray<ATile*> AllMatchingTiles;
		for (ATile* Tile : TilesToCheck)
		{
			TArray<ATile*> MatchingTiles = FindNeighbors(Tile);
			for (ATile* MatchingTile : MatchingTiles)
				AllMatchingTiles.AddUnique(MatchingTile);
		}

		/// <summary>
		/// Ÿ���� ��ġ�� ���� �ִٸ�
		/// </summary>
		if (AllMatchingTiles.Num() > 0)
		{
			SetLastMove(ETileMoveType::MT_COMBO);
			ExcuteMatch(AllMatchingTiles);
		}
		else
		{
			if (IsUnwinnable())
			{
				if (AProject3MatchGameModeBase* GameMode = Cast<AProject3MatchGameModeBase>(UGameplayStatics::GetGameMode(this)))
				{
					// ���� Ÿ�� �缳������ ���� �� ��
					GameMode->GameOver();
					return;
				}
			}

			//UMatch3BlueprintFunctionLibrary::PauseGameTimer(this, false);
		}
	}
}

void AGrid::ReturnMatchSound(TArray<USoundWave*>& MatchSounds)
{

}

int32 AGrid::GetScoreMultiplierForMove(ETileMoveType::Type LastMoveType)
{
	return 0;
}

void AGrid::OnSwapDisplayFinished(ATile* InTile)
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