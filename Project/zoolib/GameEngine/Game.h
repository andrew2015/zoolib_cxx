#ifndef __ZooLib_GameEngine_Game_h__
#define __ZooLib_GameEngine_Game_h__ 1
#include "zconfig.h"

#include "zoolib/File.h"

#include "zoolib/GameEngine/AssetCatalog.h"
#include "zoolib/GameEngine/FontCatalog.h"
#include "zoolib/GameEngine/Cog.h"
#include "zoolib/GameEngine/Sound.h"
#include "zoolib/GameEngine/Touch.h"
#include "zoolib/GameEngine/Types.h"

namespace ZooLib {
namespace GameEngine {

void sRotateGameRate();

void sSetGameRate(Rat iRate);
Rat sGetGameRate();

// =================================================================================================
#pragma mark -

class Game
:	public ZCounted
	{
public:
	Game(const FileSpec& iFS,
		const ZRef<Callable_TextureFromPixmap>& iCallable_TextureFromPixmap,
		bool iPreferProcessedArt, bool iPreferSmallArt,
		const ZRef<SoundMeister>& iSoundMeister);

	virtual ~Game();

// From ZCounted
	virtual void Initialize();

// Our protocol
	virtual void Resume() = 0;
	virtual void Pause() = 0;
	virtual void TearDown() = 0;

	virtual GPoint GetGameSize() = 0;

	void WakeToBeKilled();

	void ExternalPurgeHasOccurred();

	void Purge();

	ZRef<NookScope> GetNookScope();

	void UpdateKeyDowns(int iKey);

	void UpdateTouches(const TouchSet* iDown, const TouchSet* iMove, const TouchSet* iUp);

	void Draw(
		double iNewTimestamp,
		GPoint iBackingSize,
		const ZRef<Callable_Void>& iCallable_FlipBuffers);

	void RunOnce();

private:
	void pUpdateTouches();

	ZRef<Rendered> pCrank(double iInterval, const std::vector<int>* iKeyDowns);

	ZRef<Rendered> fRendered;
	ZRef<Rendered> fRendered_Prior;

	ZMtx fMtx_Game;
	ZCnd fCnd_Game;

	double fTimestamp_LatestDrawn;
	double fTimestamp_ToDraw;

	ZRef<NookScope> fNookScope;

	Map fRootMap;

	ZRef<AssetCatalog> fAssetCatalog;

	ZRef<FontCatalog> fFontCatalog;

	ZRef<SoundMeister> fSoundMeister;

	std::vector<int> fKeyDowns;

	std::vector<ZRef<TouchListener> > fTLs;
	std::map<ZRef<Touch>,ZRef<TouchListener> > fExclusiveTouches;

	TouchSet fTouchesActive;
	TouchSet fTouchesUp;

	TouchSet fPendingTouchesDown;
	TouchSet fPendingTouchesMove;
	TouchSet fPendingTouchesUp;

	uint64 fNextEra;
	double fAccumulated;
	Cog fCog;
	};

ZRef<Game> sMakeGame(const FileSpec& iResourceFS, bool iPreferSmallArt);

} // namespace GameEngine
} // namespace ZooLib

#endif // __GameEngine_Game_h__
