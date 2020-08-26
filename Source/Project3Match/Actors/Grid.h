#ifndef __U3MATCH_GRID__
#define __U3MATCH_GRID__

#include "GameFramework/Actor.h"
#include "PaperSprite.h"
//#include "Tile.h"
#include "../MISC/GridStructure.h"
#include "../MISC/EnumLibrary.h"

#include "Grid.generated.h"


class ATile;

using ETileMovingType = ETileMoveType::Type;

UCLASS()
class PROJECT3MATCH_API AGrid : public AActor
{
	GENERATED_BODY()
public:
	AGrid(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<class ATile*> GameTiles;
	
	UPROPRTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTileType> TileLibrary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D TileSize;

	/** �� ���� �ּ� ����� �� Ÿ�� ��*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		int32 MinimumRunLength;

	/** Grid ��, Ÿ���� ��ġ�� ��� �� �� ���*/
	UPROPERTU(EditAnywhere, BlueprintReadWrite, Category = Tile)
		int32 GridWidth;

	/** Grid ��*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		int32 GirdHeight;

	/**
	@Param SpawGridAddress : Grid Index
	*/
	ATile* CreateTile(TSubclassOf<class ATile> UClassOfTile, class UMaterialInstanceConstant* TileMaterial,
		FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID);

	/** Ÿ���� Probability �Ӽ��� ������� Grid Library���� �������� �ϳ� ����, */
	int32 SelectTileFromLibrary();

	ATile* GetTileFromGridAddress(int32 GridAddress) const;

	UFUNCTION(BlueprintCallable, Category = Initialization)
		void InitGrid();

	UFUNCTION(BlueprintImplementableEvent, meta = (ExpandEnumAsExecs = MoveType), Category = Tile)
		void OnMoveMade(ETileMovingType  MoveType);

	UFUNCTION(BlueprintCallable, Cateogyr = Audio)
		void ReturnMatchSound(TArray<class USoundMove*>& MatchSounds);

	UFUNCTION(BlueprintCallable, Category = Tile)
		FVector GetLocationFromGridAddress(int32 GridAddress) const;
	
	UFUNCTION(Category = Tile)
		FVector GetLocationFromGridAddressWithOffset(int32 GridAddress, in32 XOffsetInTiles, int32 YOffsetInTiles) const;

	UFUNCTION(BlueprintCallable, Category = Tile)
		bool GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset, int32& ReturnGridAddress) const;

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
	void SwapTiles(ATile* From, ATile* To, bool bRepositionTileActor = false);

	/**
	* �ش� Ÿ�� ��ġ�� �̵� �� �� �ִ��� ���� ����
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
	TArray<ATile*> FindNeighbors(ATile* StartingTile, boll bMustMatchID = true, int32 RunLegnth = -1) const;

	TArray<ATile*> FindTilesOfType(int32 TileTypeID);
	
	void ExcuteMatch(const TArray<ATile*>& MatchingTiles);
	void OnTileWasSelecting(ATile* NewSelectedTile);
	/// <summary>
	/// ���� Ÿ���� ���տ��� ���� �� ���� ���
	/// </summary>
	/// <returns></returns>
	bool IsUnwinnable();

	void SetLastMove(ETileMovingType MoveType);
	ETileMovingType GetLastMove();

	UFUNCTION(BlueprintCallable, Category = Game)
		int32 GetScoreMultiplierForMove(ETileMovingType LastMoveType);

	inline ATile* GetCurrentlySelectedTile()const { return CurrentlySelectedTile; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tile)
		ATile* CurrentlySelectedTile;

private:
	TArray<ATile*> LastLegalMatch;
	TArray<ATile*> FallingTiles;
	TArray<ATile*> SawppingTiles;
	TArray<ATile*> TilesToCheck;
	TArray<ATile*> TilesBeingDestroyed;
	TMap<class APlayerController*, ETileMovingType> LastMoves;
	uint32 bPendingSwapMove : 1;
	uint32 bPendingSwapMoveSuccess : 1;
};


#endif // !__U3MATCH_GRID__
