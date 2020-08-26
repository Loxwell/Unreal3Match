#pragma once

#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
namespace ETileMoveType
{
	enum EType
	{
		MT_NONE,
		MT_FAILURE,
		MT_STANDARD,
		MT_MORE_TILES,
		MT_COMBO,
		MT_BOMB,
		MT_ALL_THE_BOMBS,
		MT_MAX
	};
}


