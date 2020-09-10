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
	// 좌하단
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
* @param InitialGridAddress : 검색 할 Tile 시작 Index
*/
bool AGrid::GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset, int32& ReturnGridAddress) const
{
	int32 NewAxisValue = 0;
	// 영역 밖 위치로 초기화
	// 호출 한 곳에서 해당 값 법위 확인
	ReturnGridAddress = -1;

	// Check for going off the map in the X direction.
	// X 방향으로 영역을 벗어 나는지 확인
	check(GridWidth > 0);

	NewAxisValue = (InitialGridAddress % GridWidth) + XOffset;
	// 0번째 부터 (가로 개수 - 1) 사이 값
	if(NewAxisValue != FMath::Clamp(NewAxisValue, 0, (GridWidth - 1)))
		return false;
	
	NewAxisValue = (InitialGridAddress / GridWidth) + YOffset;
	// 0번째 부터 (세로 개수 - 1 사이) 값
	if (NewAxisValue != FMath::Clamp(NewAxisValue, 0, (GridHeight - 1)))
		return false;

	// GameTiles가 1차 배열 기준 계산 방식
	// 시작 Index로 부터 가로 XOffset 만큼 우로 이동
	// XOffset 만큼 이동 한 위치 부터 가로 방향으로 YOffset * GridWidth 만큼 Shift
	ReturnGridAddress = (InitialGridAddress + XOffset + (YOffset * GridWidth));
	check(ReturnGridAddress >= 0);
	check(ReturnGridAddress < (GridWidth * GridHeight));

	return true;
}

bool AGrid::AreAddressesNeighbors(int32 GridAddressA, int32 GridAddressB) const
{
	if ((FMath::Min(GridAddressA, GridAddressB) >= 0) // 둘다 index 0 보다 큰 양수 Index
		&& (FMath::Max(GridAddressA, GridAddressB) < (GridWidth * GridHeight))) // Width * Height 보다 작은 양수 Index
	{
		int32 GridAddressOffset = FMath::Abs(GridAddressA - GridAddressB);

		// 두 Index가 연결 또는 끝과 끝 (0 - Width or Width - 0) 일 경우
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
		// 해당 Index에는 다른 Tile이 배치 되었는데 참조는 현재 인자값 Tile을 가르킬 경우
		if (GameTiles[ReturnGridAddress] == Tile)
			GameTiles[ReturnGridAddress] = nullptr;
	}

	// Validate new grid address and replace whatever is there.
	// 유효한 Tile Index 및 해당 Tile 대신 떨어진 Tile로 교체
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
	// 타일이 전부 이동이 완료 된 경우, 상단 부터 비어 있는 Column에 새로운 Tile 생성
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
		//  ATile::OnMatched_Implementation함수 안에서 AGrid::OnTileFinishedMatching을 호출 함
		// OnTileFinishedMatching함수를 호출 한 InTile 객체를 목록에서 제거
		TilesBeingDestroyed.RemoveSwap(InTile);
		// OnTileFinishedMatching()함수를 호출 한 타일객체를 삭제
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
			// BaseAddress 및 TestAddress 평가
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
		/// 타일이 일치한 줄이 있다면
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
					// 추후 타일 재설정으로 변경 할 것
					GameMode->GameOver();
					return;
				}
			}
			UMatch3BPFunctionLibrary::PauseGameTimer(this, false);
		}
	}
}

void AGrid::SwapTiles(ATile* A, ATile* B, bool bRepositionTileActors/*=false 테스트 용도로 함수 호출 할 경우, 반대는 true*/)
{
	// A,B 서로 Grid Index 교환
	int32 GridAddress = A->GetGridAddress();
	A->SetGridAddress(B->GetGridAddress());
	B->SetGridAddress(GridAddress);

	// 변경 된 Index로 배열 참조 교환
	GameTiles[A->GetGridAddress()] = A;
	GameTiles[B->GetGridAddress()] = B;

	if (bRepositionTileActors) // 변경 된 Index의 위치로 변경
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
			// 서로의 Index(Grid Address) 교환
			SwapTiles(A, B);
			LastLegalMatch = FindNeighbors(A);
			LastLegalMatch.Append(FindNeighbors(B));

			//원상 복귀
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

	// 특이 사항 처리
	if (RunLegnth == 0)
		return AllMatchingTiles;
	else if (RunLegnth == 1)
	{
		AllMatchingTiles.Add(StartingTile);
		return AllMatchingTiles;
	}

	// 시작 위치 기준 좌우 Tile 확인
	for (int32 H = 0; H < 2; ++H)
	{
		// 세로부터 검색 후 가로 검색
		int32 MaxGridOffset = (!bMustMatchID) ? RunLegnth /* 최소값 2 이상 */ : (H ? GridWidth : GridHeight);
		
		for (int32 Dir = -1; Dir <= 1; Dir += 2/* -1에서 한칸 건너 띄어서 1*/)
		{
			// 최소 한번은 실행 보장
			// bMustMatchID == true ? ( H == 0 : GridHeight ) : RunLegnth
			// bMustMatchID == true ? ( H == 1 : GridWidth ) : RunLegnth
			for (int32 GridOffset = 1; GridOffset < MaxGridOffset; ++GridOffset)
			{
				// H == 0 이면 상하 Grid Address(0, -1),(0, 1) => 상단에서 하단으로 이동시 Y값이 증가 또는 감소하는지 확인 바람
				// 순서는 
				// H == 0 && Dir == -1 : (0,-1)->(0,-2)->(0,-3)->(0,-4)->(0,-(MaxGridOffset{GridHeight or RunLegnth} - 1)) 세로
				// H == 0 && Dir ==  1 : (0, 1)->(0, 2)->(0, 3)->(0, 4)->(0, (MaxGridOffset{GridHeight or RunLegnth} - 1)) 세로
				// H == 1 && Dir == -1 : (-1,0)->(-2,0)->(-3,0)->(-4,0)->(-(MaxGridOffset{GridWidth or RunLegnth} - 1),0) 좌
				// H == 1 && Dir == -1 : (1, 0)->(2, 0)->(3, 0)->(4, 0)->((MaxGridOffset{GridWidth or RunLegnth} - 1), 0) 우
				
				// 타일 타입이 일치 하지 않을 경우 또는 경계를 벗어나면 중지
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
		// Column 일치 타일들 추가 후 Row 일치 한 타일들 추가
		// MatchInProgress.Num() : StartingTile의 ID와 일치한 타일의 개수
		// 1 : StartingTile
		if ((MatchInProgress.Num() + 1) >= FMath::Min(RunLegnth, H ? GridWidth : GridHeight) || !bMustMatchID)
			AllMatchingTiles.Append(MatchInProgress);
		MatchInProgress.Empty();
	}

	// If we found any other tile, or if we're not concerned with matching TileID, then we know we have a valid run, and we need to add the original tile to the list.
	// If we do care about matching tile type and we haven't found anything by this point, then we don't have a match and should not return the starting tile in a list by itself.
	if ((AllMatchingTiles.Num() > 0 || !bMustMatchID))
		AllMatchingTiles.Add(StartingTile); // 시작 타일과 일치한 타일들이 있을 경우, 시작 타일도 포함

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
/// 일치한 타일 삭제 및 점수 득점
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
			// 매칭 된 타일을 기준으로 Y축 이동
			if (GetGridAddressWithOffset(MatchingItem->GetGridAddress(), 0, YOffset, NextAddressUp))
			{
				NextTileUp = GetTileFromGridAddress(NextAddressUp);

				// 매칭 된 타일의 윗 타일이 일치하는 타일 목록 내 없을 경우
				if (NextTileUp && !MatchingTiles.Contains(NextTileUp)) 
				{
					// 상태를 Falling으로 변경
					NextTileUp->SetTileState(ETileState::ETS_FALLING);
					// FallingTiles 목록에 이미 등록 되어 있는지 확인
					check(!FallingTiles.Contains(NextTileUp));
					FallingTiles.Add(NextTileUp);
					continue;
				}
				break;
			}
		}
		// 매칭 목록에 있는 타일을 삭제 대기 상태로 변경
		MatchingItem->SetTileState(ETileState::ETS_PENDING_DELETE);
	}

	// 이동 할 타일 수와 일치 되서 삭제 될 타일 수를 기반으로 
	// 보드를 다시 채울 타일 수 확인 단계 (삭제 된 타일 수 만 재 생성)
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
				// 기존의 누적 된 콤보는 초기화
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
			// 매칭 된 타일 삭제 -> 메모리 풀 방식으로 변경 할 것
			TilesBeingDestroyed.Add(MatchingItem);
			GameTiles[MatchingItem->GetGridAddress()] = nullptr;
			MatchingItem->OnMatched(GetLastMove());
		}

		// 삭제 할 타일이 없는 경우도 확인
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

			// 유효 타일 처리
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

	// 현재 이동 중인 타일, 일치한 타일이 있을 경우, 현재 GamePlay 상태가 정지 상태, 선택 한 타일이 Null 인 경우
	if (bPendingSwapMove || FallingTiles.Num() || TilesBeingDestroyed.Num() ||
		!UMatch3BPFunctionLibrary::IsGameActive(this) || !NewSelectedTile)
		return;

	FTileType& NewSelectedTileType = TileLibrary[NewSelectedTile->GetTileTypeID()];

	if (CurrentlySelectedTile)
	{
		// 이미 선택 된 타일과 현재 선택한 타일이 서로 이웃 인지 확인
		if (AreAddressesNeighbors(CurrentlySelectedTile->GetGridAddress(), NewSelectedTile->GetGridAddress()))
		{
			if (NewSelectedTileType.Abilities.CanSwap())
			{
				bPendingSwapMove = true;
				bPendingSwapMoveSuccess = (IsMoveLegal(CurrentlySelectedTile, NewSelectedTile));
				// 이동 할 타일과 이동 될 타일을 서로 위치 교환
				CurrentlySelectedTile->OnSwapMove(NewSelectedTile, bPendingSwapMoveSuccess);
				NewSelectedTile->OnSwapMove(CurrentlySelectedTile, bPendingSwapMoveSuccess);
			}
			else
				// 두번째 타일이 Movable 타입이 아니라서 이동 불가
				// 이동 하려고 했던 타일을 선택 취소 처리
				OnMoveMade(ETileMoveType::MT_FAILURE);
		}

		// 선택 된 타일에 이벤트는 발생이 완료 된 시점이라 더 이상 현재 선택 중인 상태를 초기화 한다
		CurrentlySelectedTile->PlaySelectionEffect(false);
		CurrentlySelectedTile = nullptr;
	}
	else
	{
		// 선택 된 타일(들)의 특별한 기능 확인
		// 추후 폭탄 이외 다양한 기능을 추가 할 것
		// 특수 기능에 따라 함수포인터 또는 딜리게이트로 반환 받아서 함수 호출 하는 방식으로 변경 할 것
		if (NewSelectedTileType.Abilities.CanExplode())
		{
			TArray<ATile*> TilesToDestroy;
			if (A3MatchGameMode* GameMode = Cast<A3MatchGameMode>(UGameplayStatics::GetGameMode(this)))
			{
				if (GameMode->GetComboPower() == GameMode->GetMaxComboPower())
				{
					// 모든 폭탄 타일을 한번에 폭발 유발
					SetLastMove(ETileMoveType::MT_ALL_THE_BOMBS);

					TArray<ATile*> SpecialTiles = FindTilesOfType(NewSelectedTile->GetTileTypeID());

					TArray<ATile*> TileToDestroyForCurrentSpecialTiles;
					for (ATile* Tile : SpecialTiles)
					{
						TileToDestroyForCurrentSpecialTiles = GetExplosionList(Tile);
						for (ATile* TileToCheck : TileToDestroyForCurrentSpecialTiles)
						{
							// 중복 방지
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

		// 특수 타일(폭탄 등)은 항상 유효
		if (Tile->Abilities.CanExplode())
			return false;

		for(int32 W = 0; W < 2; ++W)
			for (int32 H = 0; H < 2; ++H)
			{
				int32 XOffset = (!W ? 0 : 1) * (!H ? -1 : 1);
				int32 YOffset = (!W ? 1 : 0) * (!H ? -1 : 1);
				// If any tile can move in any direction, then the game is not unwinnable.
				// 어떤 방향이든 이동 가능 하다면, 게임은 계속 진행 가능
				if (GetGridAddressWithOffset(TileGridAddress, XOffset, YOffset, NeighborGridAddress)
					&& IsMoveLegal(Tile, GetTileFromGridAddress(NeighborGridAddress)))
					return false;
			}
	}

	/// <summary>
	/// 특수 타일 및 이동 가능한 타일이 없음.
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
		// Key = 현재 게임 체험자의 PlayerController 
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