
#pragma once
//#include "CoreMinimal.h"
#include "HAL/Platform.h"
#include "UObject/ObjectMacros.h"
#include "TileStructure.generated.h"

USTRUCT(BlueprintType)
struct FTileAbilities
{
	GENERATED_USTRUCT_BODY()

public:
	FTileAbilities()
		:bExplodes(0), bPreventSwapping(0)
	{

	}

	bool CanExplode() { return bExplodes; }
	bool CanSweap() { return !bPreventSwapping && !bExplodes; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 BoobPower = 0;

protected:
	/**
	������ Ÿ���� ����
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint32 bExplodes : 1;
	/**
	* ��ġ ��ȯ �Ұ�
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint32 bPreventSwapping : 1;
};