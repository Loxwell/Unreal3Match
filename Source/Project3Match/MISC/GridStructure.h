
#ifndef __U3MATCH_GRID_STRUCT__
#define __U3MATCH_GRID_STRUCT__
#include "Templates/SubclassOf.h"
#include "Math/Color.h"
#include "TileStructure.h"

namespace Grid
{
	USTRUCT(BlueprintType)
	struct FTileType
	{
		GENERATED_USTRUCT_BODY();

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			class UMaterialInstanceConstance* TileMaterial = nullptr;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<class ATile> TileClass;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			FLinearColor EffectColor = FLinearColor::White;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			FTileAbilities Abilities;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			float Probability = 1.f;
	};
}
#endif // !__U3MATCH_GRID_STRUCT__

