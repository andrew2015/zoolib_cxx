// Copyright (c) 2019 Andrew Green. MIT License. http://www.zoolib.org

#ifndef __ZooLib_GameEngine_Cog_h__
#define __ZooLib_GameEngine_Cog_h__ 1

#include "zoolib/Cog.h"

#include "zoolib/GameEngine/AssetCatalog.h"
#include "zoolib/GameEngine/FontCatalog.h"
#include "zoolib/GameEngine/Nook.h"
#include "zoolib/GameEngine/Rendered.h"
#include "zoolib/GameEngine/Touch.h"
#include "zoolib/GameEngine/Types.h"
#include "zoolib/GameEngine/Util_Allocator.h"

#include "zoolib/ZMACRO_foreach.h"

#include <map>
#include <vector>

namespace ZooLib {
namespace GameEngine {

// =================================================================================================
#pragma mark - InChannel

struct InChannel
	{
	InChannel(GPoint iGameSize, const std::vector<int>* iKeyDowns, const ZP<NookScope>& iNookScope);

	InChannel(const InChannel& iParent, const ZP<NookScope>& iNookScope);

	InChannel(const InChannel& iParent, const Mat& iMat);

	const InChannel* GetPrior() const;

	GPoint GetGameSize() const;

	const std::vector<int>* GetKeyDowns() const;

	const ZP<NookScope>& GetActiveNookScope() const;

	const ZP<NookScope>& GetOwnNookScope() const;

	const Mat& GetMat() const;

private:
	const InChannel* fPrior;
	const GPoint fGameSize;
	const std::vector<int>* fKeyDowns;
	ZP<NookScope> fNookScope;
	Mat fMat;
	};

// =================================================================================================
#pragma mark - OutChannel

class OutChannel
:	ZooLib::NonCopyable
	{
public:
	OutChannel(
		const Map& iRootMap,
		const ZP<AssetCatalog>& iAssetCatalog,
		const ZP<FontCatalog>& iFontCatalog,
		const ZP<SoundMeister>& iSoundMeister,
		const ZP<Callable_Void>& iHaptic);

	~OutChannel();

	std::vector<ZP<TouchListener> >& GetTLs() const;
	ZP<Rendered_Group>& GetGroup() const;

	void RegisterTouchListener(ZP<TouchListener> iListener) const;

	const Map fRootMap;
	const ZP<AssetCatalog> fAssetCatalog;
	const ZP<FontCatalog> fFontCatalog;
	const ZP<SoundMeister> fSoundMeister;
	const ZP<Callable_Void> fHaptic;

private:
	mutable std::vector<ZP<TouchListener> > fTLs;
	mutable ZP<Rendered_Group> fGroup;
	};

// =================================================================================================
#pragma mark - Param and Cog

struct Param
	{
	Param(const InChannel& iInChannel, const OutChannel& iOutChannel,
		uint64 iEra,
		double iElapsed, double iInterval, double iSubsequentTime)
	:	fEra(iEra)
	,	fInChannel(iInChannel)
	,	fOutChannel(iOutChannel)
	,	fElapsed(iElapsed)
	,	fInterval(iInterval)
	,	fSubsequentTime(iSubsequentTime)
		{}

	Param(const Param& iParam, const InChannel& iInChannel)
	:	fEra(iParam.fEra)
	,	fInChannel(iInChannel)
	,	fOutChannel(iParam.fOutChannel)
	,	fElapsed(iParam.fElapsed)
	,	fInterval(iParam.fInterval)
	,	fSubsequentTime(iParam.fSubsequentTime)
		{}

	uint64 GetEra() const
		{ return fEra; }

	const uint64 fEra;
	const InChannel& fInChannel;
	const OutChannel& fOutChannel;
	const double fElapsed;
	const double fInterval;
	const double fSubsequentTime;
	};

typedef ZooLib::Cog<const Param&> Cog;

// =================================================================================================
#pragma mark - Nooks

ZP<Nook> sGetOneNook(const Param& iParam, const Name& iName);

std::vector<ZP<Nook> > sGetAllNooks(const Param& iParam, const Name& iName);

template <class Nook_p>
ZP<Nook_p> sGetOneNook(const Param& iParam)
	{ return sGetOneNook(iParam, typeid(Nook_p).name()).DynamicCast<Nook_p>(); }

template <class Nook_p>
std::vector<ZP<Nook_p> > sGetAllNooks(const Param& iParam)
	{
	std::vector<ZP<Nook_p> > result;
	foreacha (entry, sGetAllNooks(iParam, Name(typeid(Nook_p).name())))
		{
		if (ZP<Nook_p> theNook = entry.template DynamicCast<Nook_p>())
			result.push_back(theNook);
		}
	return result;
	}

template <class Nook_p>
ZP<Nook_p> sUpdateOneNook(const Param& iParam, WP<Nook_p>& ioWP)
	{
	ZP<Nook_p> theNook = ioWP;
	if (not theNook)
		{
		theNook = sGetOneNook<Nook_p>(iParam);
		ioWP = theNook;
		}
	return theNook;
	}

template <class Nook_p>
void sUpdateNookSet(const Param& iParam, NookSet<Nook_p>& ioNookSet)
	{
	ioNookSet.fNookWPs.clear();
	foreacha (entry, sGetAllNooks(iParam, Name(typeid(Nook_p).name())))
		{
		if (ZP<Nook_p> theNook = entry.template DynamicCast<Nook_p>())
			ioNookSet.fNookWPs.insert(sWP(theNook));
		}
	}

// =================================================================================================
#pragma mark - CogRegistration

class CogRegistration
	{
public:
	typedef Cog(*Fun)(const Map& iMap);

	CogRegistration(const string8& iCtorName, Fun iFun);

	static Cog sCtor(const string8& iCtorName, const Map& iMap);
	};

// =================================================================================================
#pragma mark - sCog

Cog sCog(const ZQ<Val>& iValQ);

// =================================================================================================
#pragma mark -

Cog sCog_NewEpoch(const Cog& iChild);

Cog sCog_Noop();

// =================================================================================================
#pragma mark - Touches

Cog sCog_UpdateTouchListener(const ZP<TouchListener>& iTouchListener);
Cog sCog_TouchNothing(const ZP<TouchListener>& iTouchListener);
Cog sCog_TouchDown(const ZP<TouchListener>& iTouchListener);

Cog sCog_DisableTouches(const Cog& iCog);

// =================================================================================================
#pragma mark - Logging cogs

Cog sCog_Log1(int iLevel, const string8& iMessage);
Cog sCog_Log1(const string8&);
Cog sCog_Log(const string8&);

// =================================================================================================
#pragma mark - Larger collections of cogs

Cog sCogs_Plus(std::vector<Cog>* ioCogs);
Cog sCogs_Each(std::vector<Cog>* ioCogs);

// =================================================================================================
#pragma mark -

struct CogAccumulatorCombiner_Plus
	{ void operator()(Cog& io0, const Cog& i1) const; };

struct CogAccumulatorCombiner_Minus
	{ void operator()(Cog& io0, const Cog& i1) const; };

struct CogAccumulatorCombiner_Each
	{ void operator()(Cog& io0, const Cog& i1) const; };

struct CogAccumulatorCombiner_And
	{ void operator()(Cog& io0, const Cog& i1) const; };

struct CogAccumulatorCombiner_Or
	{ void operator()(Cog& io0, const Cog& i1) const; };

} // namespace GameEngine
} // namespace ZooLib

#endif // __ZooLib_GameEngine_Cog_h__
