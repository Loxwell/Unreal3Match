#pragma once

#include "GameFramework/Actor.h"
#include "PaperSprite.h"
//#include "HAL/Platform.h"
//#include "Tile.h"
#include "../MISC/GridStructure.h"
#include "../MISC/EnumLibrary.h"

#include "Grid.generated.h"

class ATile;

UCLASS()
class PROJECT3MATCH_API AGrid : public AActor
{
	GENERATED_BODY()
public:
	AGrid(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = Initialization)
		void InitGrid();

	UFUNCTION(BlueprintImplementableEvent, meta = (ExpandEnumAsExecs = "MoveType"), Category = Tile)
		void OnMoveMade(ETileMoveType::Type MoveType);

	UFUNCTION(BlueprintCallable, Category = Audio)
		void ReturnMatchSound(TArray<class USoundWave*>& MatchSounds);

	UFUNCTION(BlueprintCallable, Category = Tile)
		FVector GetLocationFromGridAddress(int32 GridAddress) const;

	UFUNCTION(Category = Tile)
		FVector GetLocationFromGridAddressWithOffset(int32 GridAddress, int32 XOffsetInTiles, int32 YOffsetInTiles) const;

	UFUNCTION(BlueprintCallable, Category = Tile)
		bool GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset, int32& ReturnGridAddress) const;

	UFUNCTION(BlueprintCallable, Category = Game)
		int32 GetScoreMultiplierForMove(ETileMoveType::Type LastMoveType);

	/**
	@Param SpawGridAddress : Grid Index
	*/
	ATile* CreateTile(TSubclassOf<class ATile> UClassOfTile, class UMaterialInstanceConstant* TileMaterial,
		FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID);

	/** 타일의 Probability 속성을 기반으로 Grid Library에서 랜덤으로 하나 뽑음, */
	int32 SelectTileFromLibrary();

	ATile* GetTileFromGridAddress(int32 GridAddress) const;

	bool AreAddressNeighbors(int32 GridAddress, int32 GridAddressB) const;
	
	/// <summary>
	/// Moving
	/// </summary>
	/// <param name="Tile"></param>
	/// <param name="LandingGridAddress"></param>
	void OnTileFinishedFalling(ATile* Tile, int32 LandingGridAddress);
	
	/// <summary>
	/// Compare neighbors tiles
	/// </summary>
	void OnTileFinishedMatching(ATile* InTile);
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="InTile"></param>
	void OnSwapDisplayFinished(ATile* InTile);

	void RespawnTiles();
	void SwapTiles(ATile* From, ATile* To, bool bRepositionTileActors = false);

	/**
	* 해당 타일 위치로 이동 할 수 있는지 사전 시험
	*/
	bool IsMovingLegal(ATile* Form, ATile* To);

	TArray<ATile*> GetExploionList(ATile* Tile) const;
	/// <summary>
	/// 
	/// </summary>
	/// <param name="StartingTile"></param>
	/// <param name="bMustMatchID"></param>
	/// <param name="RunLegnth"></param>
	/// <returns></returns>
	TArray<ATile*> FindNeighbors(ATile* StartingTile, bool bMustMatchID = true, int32 RunLegnth = -1) const;

	TArray<ATile*> FindTilesOfType(int32 TileTypeID);
	
	void ExcuteMatch(const TArray<ATile*>& MatchingTiles);

	void OnTileWasSelecting(ATile* NewSelectedTile);
	/// <summary>
	/// 현재 타일의 조합에서 맞출 수 없는 경우
	/// </summary>
	/// <returns></returns>
	bool IsUnwinnable();

	void SetLastMove(ETileMoveType::Type MoveType);

	ETileMoveType::Type GetLastMove();

	inline ATile* GetCurrentlySelectedTile()const { return CurrentlySelectedTile; }

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<class ATile*> GameTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTileType> TileLibrary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D TileSize;

	/** 행 기준 최소 맞춰야 할 타일 수*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		int32 MinimumRunLength;

	/** Grid 행, 타일의 위치를 계산 할 때 사용*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		int32 GridWidth;

	/** Grid 열*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		int32 GridHeight;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tile)
		ATile* CurrentlySelectedTile;

private:
	TArray<ATile*> LastLegalMatch;
	TArray<ATile*> FallingTiles;
	TArray<ATile*> SawppingTiles;
	TArray<ATile*> TilesToCheck;
	TArray<ATile*> TilesBeingDestroyed;
	TMap<class APlayerController*, ETileMoveType::Type> LastMoves;
	uint32 bPendingSwapMove : 1;
	uint32 bPendingSwapMoveSuccess : 1;
};


