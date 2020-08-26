#pragma once
#include "PaperSpriteActor.h"
#include "PaperSpriteComponent.h"
#include "../MISC/TileStructure.h"
#include "Tile.generated.h"

UCLASS()
class PROJECT3MATCH_API ATile : public APaperSpriteActor
{
	GENERATED_BODY()

public:
	UFUNCTION()
		virtual void SetTileMaterial(class UMaterialInstanceConstant* TileMaterial);

	void SetGridAddress(int32 NewAddressLocation);
	inline int32 GetTileTypeID()const { return TileTypeID; }
	inline void SetTileTypeID(const int32& NewTypeID) { TileTypeID = NewTypeID; }

public:
	UPROPERTY(BlueprintReadOnly)
		FTileAbilities Abilities;

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		int32 TileTypeID;
};