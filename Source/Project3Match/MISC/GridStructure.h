
#pragma once

#include "Templates/SubclassOf.h"
#include "UObject/ObjectMacros.h"

#include "Math/Color.h"
#include "TileStructure.h"
#include "GridStructure.generated.h"

USTRUCT(BlueprintType)
struct FTileType
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterialInstanceConstant* TileMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ATile> TileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor EffectColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTileAbilities Abilities;

	/*È®·ü, °¡´É¼º*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Probability = 1.f;
};

