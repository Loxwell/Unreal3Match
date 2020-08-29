#pragma once
#include "PaperSpriteActor.h"
#include "PaperSpriteComponent.h"
#include "../MISC/TileStructure.h"
#include "../MISC/EnumLibrary.h"
#include "Tile.generated.h"

#define SPECIAL_TYPE "Special Game Events"

UCLASS()
class PROJECT3MATCH_API ATile : public APaperSpriteActor
{
	GENERATED_BODY()

public:
	UFUNCTION()
		virtual void SetTileMaterial(class UMaterialInstanceConstant* TileMaterial);
	UFUNCTION(BlueprintNativeEvent, Category = SPECIAL_TYPE)
		void OnSwapMove(ATile * OtherTile, bool bMoveWillSuccess);
	virtual void OnSwapMove_Implementation(ATile* OtherTile, bool bMoveWillSuccess);

	// �̺�Ʈ �޽��� ��� ������ ���� ã�� �� ��
	UFUNCTION(BlueprintImplementableEvent, Category = SPECIAL_TYPE)
		void PlaySelectionEffect(bool bTurnEffectOn);

	void StartFalling(bool bUseCurrentWorldLocation = false);
	void OnMatched(ETileMoveType::Type MoveType);

	inline int32 GetGridAddress() const { return GridAddress; }
	inline void SetGridAddress(int32 NewAddressLocation);

	inline int32 GetTileTypeID()const { return TileTypeID; }
	inline void SetTileTypeID(const int32& NewTypeID) { TileTypeID = NewTypeID; }

	inline void SetTileState(const TEnumAsByte<ETileState::Type>& NewState) { TileState = NewState; }

public:
	UPROPERTY(BlueprintReadOnly)
		FTileAbilities Abilities;

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		int32 TileTypeID;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		int32 GridAddress;
	UPROPERTY()
		TEnumAsByte<ETileState::Type> TileState;
};