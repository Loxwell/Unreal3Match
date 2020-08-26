#ifndef __U3MATCH_ENUM_MOVE_TYPE__
#define __U3MATCH_ENUM_MOVE_TYPE__

#include "UObject/ObjectMacros.h"

namespace ETileMoveType
{
	UENUM(BlueprintType)
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
#endif // !__U3MATCH_ENUM_MOVE_TYPE__


