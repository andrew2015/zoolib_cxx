// Copyright (c) 2019 Andrew Green. MIT License. http://www.zoolib.org

#ifndef __ZooLib_GameEngine_Cog_Toon_h__
#define __ZooLib_GameEngine_Cog_Toon_h__ 1

#include "zoolib/GameEngine/Cog.h"
#include "zoolib/GameEngine/Toon.h"

namespace ZooLib {
namespace GameEngine {

Cog sCog_Toon_NoEpoch(const ZP<Toon>& iToon);

Cog sCog_Toon(const ZP<Toon>& iToon);

} // namespace GameEngine
} // namespace ZooLib

#endif // __ZooLib_GameEngine_Cog_Toon_h__
