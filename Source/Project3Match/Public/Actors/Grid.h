#pragma once

#include "GameFramework/Actor.h"
#include "PaperSprite.h"
//#include "HAL/Platform.h"
//#include "Tile.h"
#include "../MISC/GridStructure.h"
#include "../MISC/EnumLibrary.h"

#include "Grid.generated.h"

#define DEFAULT_VALUE 100

class ATile;

UCLASS()
class PROJECT3MATCH_API AGrid : public AActor
{
	GENERATED_BODY()

public:
	/*
	@Summary : In Game���� ���� ���� �� ���� Ÿ��
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<class ATile*> GameTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTileType> TileLibrary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D TileSize;

	/** ���ٷ� ��ġ �ϴ� Ÿ���� �ּҼ�*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		int32 MinimumRunLength;

	/** Grid ��, Ÿ���� ��ġ�� ��� �� �� ���*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		int32 GridWidth;

	/** Grid ��*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		int32 GridHeight;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tile)
		ATile* CurrentlySelectedTile;

private:
	TArray<ATile*> LastLegalMatch;
	TArray<ATile*> FallingTiles;
	TArray<ATile*> SwappingTiles;
	TArray<ATile*> TilesToCheck;
	TArray<ATile*> TilesBeingDestroyed;
	TMap<class APlayerController*, ETileMoveType::Type> LastMoves;
	uint32 bPendingSwapMove : 1;
	uint32 bPendingSwapMoveSuccess : 1;

public:
	AGrid(const FObjectInitializer& ObjectInitializer);

	/// <summary>
	/// ���� ��� ����Ʈ���� ȣ��
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Initialization)
		void InitGrid();

	/**��� ����Ʈ �� ���� ���� �Լ�*/
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
		virtual int32 GetScoreMultiplierForMove(ETileMoveType::Type LastMoveType);

	/**
	@Param SpawGridAddress : Grid Index
	*/
	ATile* CreateTile(TSubclassOf<class ATile> UClassOfTile, class UMaterialInstanceConstant* TileMaterial,
		FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID);

	/* Ÿ���� Probability �Ӽ��� ������� Grid Library���� �������� �ϳ� ����*/
	int32 SelectTileIDFromLibrary();

	ATile* GetTileFromGridAddress(int32 GridAddress) const;
	
	/** Determine if two grid addresses are valid and adjacent. */
	bool AreAddressesNeighbors(int32 GridAddressA, int32 GridAddressB) const;
	
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
	void SwapTiles(ATile* A, ATile* B, bool bRepositionTileActors = false);

	/**
	* �ΰ��� Ÿ���� ���� ��ġ�� ��ȯ �� �� �ִ��� ����
	*/
	bool IsMoveLegal(ATile* A, ATile* B);

	TArray<ATile*> GetExplosionList(ATile* Tile) const;
	/// <summary>
	/// 
	/// </summary>
	/// <param name="StartingTile"></param>
	/// <param name="bMustMatchID"></param>
	/// <param name="RunLegnth"></param>
	/// <returns></returns>
	TArray<ATile*> FindNeighbors(ATile* StartingTile, bool bMustMatchID = true, int32 RunLegnth = -1) const;

	TArray<ATile*> FindTilesOfType(int32 TileTypeID);
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="MatchingTiles"></param>
	void ExecuteMatch(const TArray<ATile*>& MatchingTiles);

	void OnTileWasSelected(ATile* NewSelectedTile);
	/// <summary>
	/// ���� Ÿ���� ���տ��� ���� �� ���� ���
	/// </summary>
	/// <returns></returns>
	bool IsUnwinnable();

	void SetLastMove(ETileMoveType::Type MoveType);

	ETileMoveType::Type GetLastMove();

	inline ATile* GetCurrentlySelectedTile()const { return CurrentlySelectedTile; }

};


