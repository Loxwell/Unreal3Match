#include "Actors/Grid.h"
#include "Actors/Tile.h"
#include "../Project3MatchGameModeBase.h"
#include "MISC/EnumLibrary.h"
#include "MISC/Match3BPFunctionLibrary.h"

#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

#include "DrawDebugHelpers.h"

using A3MatchGameMode = AProject3MatchGameModeBase;

AGrid::AGrid(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer),
	MinimumRunLength(3)
{
	PrimaryActorTick.bCanEverTick = false;

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
			NewTile->SetTileMaterial(TileMaterial);
			NewTile->SetGridAddress(SpawnGridAddress);
			GameTiles[SpawnGridAddress] = NewTile;

#if WITH_EDITOR
			DrawDebugBox(World, SpawnLocation, FVector(TileSize.X, TileSize.Y, TileSize.Y) * .5f, FColor::Red, true);
#endif
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

	float TestNumber = FMath::FRandRange(0.f, NormailizingFactor);
	float CompareTo = 0;

	for (int32 ArrayChecked = 0, TilesNum = TileLibrary.Num(); ArrayChecked != TilesNum; ++ArrayChecked)
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
	OutLocation.Z += TileSize.Y * (float)(GridAddress / GridWidth);
	OutLocation += Center;

	return OutLocation;
}

FVector AGrid::GetLocationFromGridAddressWithOffset(int32 GridAddress, int32 XOffsetInTiles, int32 YOffsetInTiles) const
{
	FVector OutLocation = GetLocationFromGridAddress(GridAddress);
	OutLocation.X += TileSize.X * (float)(XOffsetInTiles);
	OutLocation.Z += TileSize.Y * (float)(YOffsetInTiles);

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
	// 0��° ���� (���� ���� - 1) ���� ��
	if(NewAxisValue != FMath::Clamp(NewAxisValue, 0, (GridWidth - 1)))
		return false;
	
	NewAxisValue = (InitialGridAddress / GridWidth) + YOffset;
	// 0��° ���� (���� ���� - 1 ����) ��
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
	if ((FMath::Min(GridAddressA, GridAddressB) >= 0) // �Ѵ� index 0 ���� ū ��� Index
		&& (FMath::Max(GridAddressA, GridAddressB) < (GridWidth * GridHeight))) // Width * Height ���� ���� ��� Index
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

/// <summary>
/// 
/// </summary>
/// <param name="InTile"></param>
void AGrid::OnTileFinishedMatching(ATile* InTile)
{
	if (InTile)
	{
		//  ATile::OnMatched_Implementation�Լ� �ȿ��� AGrid::OnTileFinishedMatching�� ȣ�� ��
		// OnTileFinishedMatching�Լ��� ȣ�� �� InTile ��ü�� ��Ͽ��� ����
		TilesBeingDestroyed.RemoveSwap(InTile);
		// OnTileFinishedMatching()�Լ��� ȣ�� �� Ÿ�ϰ�ü�� ����
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
	return;

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
			return;
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
			ExecuteMatch(AllMatchingTiles);
		}
		else
		{
			if (IsUnwinnable())
			{
				if (A3MatchGameMode* GameMode = Cast<A3MatchGameMode>(UGameplayStatics::GetGameMode(this)))
				{
					// ���� Ÿ�� �缳������ ���� �� ��
					GameMode->GameOver();
					return;
				}
			}
			UMatch3BPFunctionLibrary::PauseGameTimer(this, false);
		}
	}
}

void AGrid::SwapTiles(ATile* A, ATile* B, bool bRepositionTileActors/*=false �׽�Ʈ �뵵�� �Լ� ȣ�� �� ���, �ݴ�� true*/)
{
	// A,B ���� Grid Index ��ȯ
	int32 GridAddress = A->GetGridAddress();
	A->SetGridAddress(B->GetGridAddress());
	B->SetGridAddress(GridAddress);

	// ���� �� Index�� �迭 ���� ��ȯ
	GameTiles[A->GetGridAddress()] = A;
	GameTiles[B->GetGridAddress()] = B;

	if (bRepositionTileActors) // ���� �� Index�� ��ġ�� ����
	{
		A->SetActorLocation(GetLocationFromGridAddress(A->GetGridAddress()));
		B->SetActorLocation(GetLocationFromGridAddress(B->GetGridAddress()));
	}
}

TArray<ATile*> AGrid::GetExplosionList(ATile* Tile) const
{
	check(Tile);
	check(Tile->Abilities.CanExplode());
	
	int32 AdjustedBombPower = Tile->Abilities.BoobPower;
	
	if(A3MatchGameMode* GameMode = Cast<A3MatchGameMode>(UGameplayStatics::GetGameMode(Tile)))
		AdjustedBombPower = FMath::Max(1, AdjustedBombPower + 1 + GameMode->CalculateBombPower());

	return FindNeighbors(Tile, false, AdjustedBombPower);
}

bool AGrid::IsMoveLegal(ATile* A, ATile* B)
{
	if (A && B && (A != B) && A->Abilities.CanSwap() && B->Abilities.CanSwap())
	{
		if ((A->GetTileTypeID() != B->GetTileTypeID()) && AreAddressesNeighbors(A->GetGridAddress(), B->GetGridAddress()))
		{
			// ������ Index(Grid Address) ��ȯ
			SwapTiles(A, B);
			LastLegalMatch = FindNeighbors(A);
			LastLegalMatch.Append(FindNeighbors(B));

			//���� ����
			SwapTiles(A, B);
			return (LastLegalMatch.Num() > 0);
		}
	}

	return false;
}

TArray<ATile*> AGrid::FindNeighbors(ATile* StartingTile, bool bMustMatchID/* = true */, int32 RunLegnth /* = MinimumRunLength */) const
{
	int32 NeighborGridAddress = -1;
	ATile* NeighborTile = nullptr;
	TArray<ATile*> MatchInProgress;
	TArray<ATile*> AllMatchingTiles;

	if (RunLegnth < 0)
		RunLegnth = MinimumRunLength;

	// Ư�� ���� ó��
	if (RunLegnth == 0)
		return AllMatchingTiles;
	else if (RunLegnth == 1)
	{
		AllMatchingTiles.Add(StartingTile);
		return AllMatchingTiles;
	}

	// ���� ��ġ ���� �¿� Tile Ȯ��
	for (int32 H = 0; H < 2; ++H)
	{
		// ���κ��� �˻� �� ���� �˻�
		int32 MaxGridOffset = (!bMustMatchID) ? RunLegnth /* �ּҰ� 2 �̻� */ : (H ? GridWidth : GridHeight);
		
		for (int32 Dir = -1; Dir <= 1; Dir += 2/* -1���� ��ĭ �ǳ� �� 1*/)
		{
			// �ּ� �ѹ��� ���� ����
			// bMustMatchID == true ? ( H == 0 : GridHeight ) : RunLegnth
			// bMustMatchID == true ? ( H == 1 : GridWidth ) : RunLegnth
			for (int32 GridOffset = 1; GridOffset < MaxGridOffset; ++GridOffset)
			{
				// H == 0 �̸� ���� Grid Address(0, -1),(0, 1) => ��ܿ��� �ϴ����� �̵��� Y���� ���� �Ǵ� �����ϴ��� Ȯ�� �ٶ�
				// ������ 
				// H == 0 && Dir == -1 : (0,-1)->(0,-2)->(0,-3)->(0,-4)->(0,-(MaxGridOffset{GridHeight or RunLegnth} - 1)) ����
				// H == 0 && Dir ==  1 : (0, 1)->(0, 2)->(0, 3)->(0, 4)->(0, (MaxGridOffset{GridHeight or RunLegnth} - 1)) ����
				// H == 1 && Dir == -1 : (-1,0)->(-2,0)->(-3,0)->(-4,0)->(-(MaxGridOffset{GridWidth or RunLegnth} - 1),0) ��
				// H == 1 && Dir == -1 : (1, 0)->(2, 0)->(3, 0)->(4, 0)->((MaxGridOffset{GridWidth or RunLegnth} - 1), 0) ��
				
				// Ÿ�� Ÿ���� ��ġ ���� ���� ��� �Ǵ� ��踦 ����� ����
				if (GetGridAddressWithOffset(StartingTile->GetGridAddress(), Dir * (H ? GridOffset : 0), Dir * (H ? 0 : GridOffset), NeighborGridAddress))
				{
					NeighborTile = GetTileFromGridAddress(NeighborGridAddress);
					if (NeighborTile && ((NeighborTile->GetTileTypeID() == StartingTile->GetTileTypeID()) || !bMustMatchID))
					{
						MatchInProgress.AddUnique(NeighborTile);
						continue;
					}
					break;
				}
			}
		}

		// See if we have enough to complete a run, or if matching wasn't required. If so, keep the tiles.
		// Note that we add 1 to our match-in-progress because the starting tile isn't counted yet.
		// Column ��ġ Ÿ�ϵ� �߰� �� Row ��ġ �� Ÿ�ϵ� �߰�
		// MatchInProgress.Num() : StartingTile�� ID�� ��ġ�� Ÿ���� ����
		// 1 : StartingTile
		if ((MatchInProgress.Num() + 1) >= FMath::Min(RunLegnth, H ? GridWidth : GridHeight) || !bMustMatchID)
			AllMatchingTiles.Append(MatchInProgress);
		MatchInProgress.Empty();
	}

	// If we found any other tile, or if we're not concerned with matching TileID, then we know we have a valid run, and we need to add the original tile to the list.
	// If we do care about matching tile type and we haven't found anything by this point, then we don't have a match and should not return the starting tile in a list by itself.
	if ((AllMatchingTiles.Num() > 0 || !bMustMatchID))
		AllMatchingTiles.Add(StartingTile); // ���� Ÿ�ϰ� ��ġ�� Ÿ�ϵ��� ���� ���, ���� Ÿ�ϵ� ����

	return AllMatchingTiles;
}

TArray<ATile*> AGrid::FindTilesOfType(int32 TileTypeID)
{
	TArray<ATile*> MatchingTiles;
	for (ATile* Tile : GameTiles)
	{
		if (Tile && Tile->GetTileTypeID() == TileTypeID)
			MatchingTiles.Add(Tile);
	}

	return MatchingTiles;
}

/// <Summary>
/// ��ġ�� Ÿ�� ���� �� ���� ����
/// </Summary>
void AGrid::ExecuteMatch(const TArray<ATile*>& MatchingTiles)
{
	if (MatchingTiles.Num() == 0)
		return;

	UMatch3BPFunctionLibrary::PauseGameTimer(this, true);

	int32 NextAddressUp = -1;
	ATile* NextTileUp = nullptr;

	for (ATile* MatchingItem : MatchingTiles)
	{
		check(MatchingItem != nullptr)

		for (int32 YOffset = 1; YOffset < GridHeight; ++YOffset)
		{
			// ��Ī �� Ÿ���� �������� Y�� �̵�
			if (GetGridAddressWithOffset(MatchingItem->GetGridAddress(), 0, YOffset, NextAddressUp))
			{
				NextTileUp = GetTileFromGridAddress(NextAddressUp);

				// ��Ī �� Ÿ���� �� Ÿ���� ��ġ�ϴ� Ÿ�� ��� �� ���� ���
				if (NextTileUp && !MatchingTiles.Contains(NextTileUp)) 
				{
					// ���¸� Falling���� ����
					NextTileUp->SetTileState(ETileState::ETS_FALLING);
					// FallingTiles ��Ͽ� �̹� ��� �Ǿ� �ִ��� Ȯ��
					check(!FallingTiles.Contains(NextTileUp));
					FallingTiles.Add(NextTileUp);
					continue;
				}
				break;
			}
		}
		// ��Ī ��Ͽ� �ִ� Ÿ���� ���� ��� ���·� ����
		MatchingItem->SetTileState(ETileState::ETS_PENDING_DELETE);
	}

	// �̵� �� Ÿ�� ���� ��ġ �Ǽ� ���� �� Ÿ�� ���� ������� 
	// ���带 �ٽ� ä�� Ÿ�� �� Ȯ�� �ܰ� (���� �� Ÿ�� �� �� �� ����)
	TilesToCheck.Reset(FallingTiles.Num() + MatchingTiles.Num());

	{
		if (AProject3MatchGameModeBase* GameMode = Cast<AProject3MatchGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			ETileMoveType::Type MoveType = GetLastMove();
			int32 ScoreMul = GetScoreMultiplierForMove(MoveType);

			switch (MoveType)
			{
			case ETileMoveType::MT_BOMB:
			case ETileMoveType::MT_ALL_THE_BOMBS:
				// ������ ���� �� �޺��� �ʱ�ȭ
				GameMode->SetComboPower(0);
				break;
			case ETileMoveType::MT_COMBO:
				GameMode->SetComboPower(FMath::Min(GameMode->GetMaxComboPower(), GameMode->GetComboPower() + 1));
				break;
			}
			OnMoveMade(MoveType);
			GameMode->AddScore(MatchingTiles.Num() * ScoreMul);
		}

		for (ATile* MatchingItem : MatchingTiles)
		{
			// ��Ī �� Ÿ�� ���� -> �޸� Ǯ ������� ���� �� ��
			TilesBeingDestroyed.Add(MatchingItem);
			GameTiles[MatchingItem->GetGridAddress()] = nullptr;
			MatchingItem->OnMatched(GetLastMove());
		}

		// ���� �� Ÿ���� ���� ��쵵 Ȯ��
		OnTileFinishedMatching(nullptr);
	}
}

void AGrid::OnSwapDisplayFinished(ATile* Tile)
{
	SwappingTiles.Add(Tile);
	if (SwappingTiles.Num() == 2)
	{
		check(SwappingTiles[0] && SwappingTiles[1]);
		bPendingSwapMove = false;
		if (bPendingSwapMoveSuccess)
		{
			SwapTiles(SwappingTiles[0], SwappingTiles[1], true);
			SwappingTiles.Reset();
			if (LastLegalMatch.Num() > MinimumRunLength)
				SetLastMove(ETileMoveType::MT_MORE_TILES);
			else
				SetLastMove(ETileMoveType::MT_STANDARD);

			// ��ȿ Ÿ�� ó��
			ExecuteMatch(LastLegalMatch);
		}
		else
		{
			SwappingTiles.Empty();
			OnMoveMade(ETileMoveType::MT_FAILURE);
		}
	}
}

void AGrid::OnTileWasSelected(ATile* NewSelectedTile)
{
	bool bGameActive = UMatch3BPFunctionLibrary::IsGameActive(this);
	UE_LOG(LogTemp, Warning, TEXT("AGrid::OnTileWasSelected : %d"), bGameActive ? 1 : 0);

	// ���� �̵� ���� Ÿ��, ��ġ�� Ÿ���� ���� ���, ���� GamePlay ���°� ���� ����, ���� �� Ÿ���� Null �� ���
	if (bPendingSwapMove || FallingTiles.Num() || TilesBeingDestroyed.Num() ||
		!UMatch3BPFunctionLibrary::IsGameActive(this) || !NewSelectedTile)
		return;

	FTileType& NewSelectedTileType = TileLibrary[NewSelectedTile->GetTileTypeID()];

	if (CurrentlySelectedTile)
	{
		// �̹� ���� �� Ÿ�ϰ� ���� ������ Ÿ���� ���� �̿� ���� Ȯ��
		if (AreAddressesNeighbors(CurrentlySelectedTile->GetGridAddress(), NewSelectedTile->GetGridAddress()))
		{
			if (NewSelectedTileType.Abilities.CanSwap())
			{
				bPendingSwapMove = true;
				bPendingSwapMoveSuccess = (IsMoveLegal(CurrentlySelectedTile, NewSelectedTile));
				// �̵� �� Ÿ�ϰ� �̵� �� Ÿ���� ���� ��ġ ��ȯ
				CurrentlySelectedTile->OnSwapMove(NewSelectedTile, bPendingSwapMoveSuccess);
				NewSelectedTile->OnSwapMove(CurrentlySelectedTile, bPendingSwapMoveSuccess);
			}
			else
				// �ι�° Ÿ���� Movable Ÿ���� �ƴ϶� �̵� �Ұ�
				// �̵� �Ϸ��� �ߴ� Ÿ���� ���� ��� ó��
				OnMoveMade(ETileMoveType::MT_FAILURE);
		}

		// ���� �� Ÿ�Ͽ� �̺�Ʈ�� �߻��� �Ϸ� �� �����̶� �� �̻� ���� ���� ���� ���¸� �ʱ�ȭ �Ѵ�
		CurrentlySelectedTile->PlaySelectionEffect(false);
		CurrentlySelectedTile = nullptr;
	}
	else
	{
		// ���� �� Ÿ��(��)�� Ư���� ��� Ȯ��
		// ���� ��ź �̿� �پ��� ����� �߰� �� ��
		// Ư�� ��ɿ� ���� �Լ������� �Ǵ� ��������Ʈ�� ��ȯ �޾Ƽ� �Լ� ȣ�� �ϴ� ������� ���� �� ��
		if (NewSelectedTileType.Abilities.CanExplode())
		{
			TArray<ATile*> TilesToDestroy;
			if (A3MatchGameMode* GameMode = Cast<A3MatchGameMode>(UGameplayStatics::GetGameMode(this)))
			{
				if (GameMode->GetComboPower() == GameMode->GetMaxComboPower())
				{
					// ��� ��ź Ÿ���� �ѹ��� ���� ����
					SetLastMove(ETileMoveType::MT_ALL_THE_BOMBS);

					TArray<ATile*> SpecialTiles = FindTilesOfType(NewSelectedTile->GetTileTypeID());

					TArray<ATile*> TileToDestroyForCurrentSpecialTiles;
					for (ATile* Tile : SpecialTiles)
					{
						TileToDestroyForCurrentSpecialTiles = GetExplosionList(Tile);
						for (ATile* TileToCheck : TileToDestroyForCurrentSpecialTiles)
						{
							// �ߺ� ����
							TilesToDestroy.AddUnique(TileToCheck);
						}
					}
				}
			}

			if (TilesToDestroy.Num() == 0)
			{
				SetLastMove(ETileMoveType::MT_BOMB);
				TilesToDestroy = GetExplosionList(NewSelectedTile);
			}
			ExecuteMatch(TilesToDestroy);
		}
		else if (NewSelectedTileType.Abilities.CanSwap())
		{
			CurrentlySelectedTile = NewSelectedTile;
			CurrentlySelectedTile->PlaySelectionEffect(true);
		}
		else
			OnMoveMade(ETileMoveType::MT_FAILURE);
	}
}

bool AGrid::IsUnwinnable()
{
	for (ATile* Tile : GameTiles)
	{
		check(Tile);
		int32 TileGridAddress = Tile->GetGridAddress();
		int32 NeighborGridAddress;

		// Ư�� Ÿ��(��ź ��)�� �׻� ��ȿ
		if (Tile->Abilities.CanExplode())
			return false;

		for(int32 W = 0; W < 2; ++W)
			for (int32 H = 0; H < 2; ++H)
			{
				int32 XOffset = (!W ? 0 : 1) * (!H ? -1 : 1);
				int32 YOffset = (!W ? 1 : 0) * (!H ? -1 : 1);
				// If any tile can move in any direction, then the game is not unwinnable.
				// � �����̵� �̵� ���� �ϴٸ�, ������ ��� ���� ����
				if (GetGridAddressWithOffset(TileGridAddress, XOffset, YOffset, NeighborGridAddress)
					&& IsMoveLegal(Tile, GetTileFromGridAddress(NeighborGridAddress)))
					return false;
			}
	}

	/// <summary>
	/// Ư�� Ÿ�� �� �̵� ������ Ÿ���� ����.
	/// </summary>
	/// <returns></returns>
	return true;
}

void AGrid::SetLastMove(ETileMoveType::Type MoveType)
{
	if (APlayerController* PC = UMatch3BPFunctionLibrary::GetLocalPlayerController(this))
	{
		// Find (or add) the entry for this PlayerController and set it to the type of move that was just made.
		// This is primarily useful for multiplayer games, but will work in single-player as well.
		// <Key, Value> Pair
		// Key = ���� ���� ü������ PlayerController 
		// Value = enum ETileMoveType 
		ETileMoveType::Type& LastMoveType = LastMoves.FindOrAdd(PC);
		LastMoveType = MoveType;
	}
}

ETileMoveType::Type AGrid::GetLastMove()
{
	// Retrieve the type of move most recently made by the given player.
	// This could be stored as a single variable instead of a TMap if we were certain that our game would never support multiplayer.
	if (APlayerController* PC = UMatch3BPFunctionLibrary::GetLocalPlayerController(this))
	{
		if (ETileMoveType::Type* MoveType = LastMoves.Find(PC))
			return *MoveType;
	}

	return ETileMoveType::Type::MT_NONE;
}

int32 AGrid::GetScoreMultiplierForMove(ETileMoveType::Type LastMoveType)
{
	return DEFAULT_VALUE;
}

void AGrid::ReturnMatchSound(TArray<USoundWave*>& MatchSounds)
{
	MatchSounds.Reset();
	if (TilesBeingDestroyed.Num() > 0)
		for (ATile* Tile : TilesBeingDestroyed)
			MatchSounds.AddUnique(Tile->GetMatchSound());
}