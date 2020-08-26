
#pragma once
#include "Templates/SubclassOf.h"
#include "Math/Color.h"
//#include "Materials/MaterialInstanceConstant.h"
#include "TileStructure.h"

USTRUCT(BlueprintType)
struct FTileType
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterialInstanceConstant* TileMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ATile> TileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor EffectColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTileAbilities Abilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Probability = 1.f;
};

