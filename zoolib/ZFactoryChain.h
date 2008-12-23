/* -------------------------------------------------------------------------------------------------
Copyright (c) 2008 Andrew Green
http://www.zoolib.org

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------------------------- */

#ifndef __ZFactoryChain__
#define __ZFactoryChain__ 1
#include "zconfig.h"

// Put one of these in the source file where you're defining the base class
// or factory functions for a chain. The assignment of sHead_XXX to sHead_XXX is
// a sneaky trick which causes the instantiation of the template, allocates
// space for sHead_XXX, and does so without initialization order problems. It
// does rely on the loader initializing vars as zero, even when they're
// going to be initialized by global static constructors.

#define ZOOLIB_FACTORYCHAIN_HEAD(Result, Param) \
	template<> \
	ZFactoryChain_T<Result, Param>* \
	ZFactoryChain_T<Result, Param>::sHead_Preferred = sHead_Preferred; \
	template<> \
	ZFactoryChain_T<Result, Param>* \
	ZFactoryChain_T<Result, Param>::sHead_Normal = sHead_Normal

NAMESPACE_ZOOLIB_BEGIN

// =================================================================================================
#pragma mark -
#pragma mark * ZFactoryChain_T

template <class Result, class Param>
class ZFactoryChain_T
	{
public:
	typedef Result Result_t;
	typedef Param Param_t;

	static bool sMake(Result& oResult, Param iParam)
		{
		// Try preferred factories first
		for (ZFactoryChain_T* iter = sHead_Preferred; iter; iter = iter->fNext)
			{
			if (iter->Make(oResult, iParam))
				return true;
			}

		// Then non-preferred
		for (ZFactoryChain_T* iter = sHead_Normal; iter; iter = iter->fNext)
			{
			if (iter->Make(oResult, iParam))
				return true;
			}
		return false;
		}

	static Result sMake(Param iParam)
		{
		Result result;
		if (sMake(result, iParam))
			return result;
		return Result();
		}

	ZFactoryChain_T(bool iPreferred)
		{
		if (iPreferred)
			{
			fNext = sHead_Preferred;
			sHead_Preferred = this;
			}
		else
			{
			fNext = sHead_Normal;
			sHead_Normal = this;
			}
		}

	virtual bool Make(Result& oResult, Param iParam)
		{ return false; }

private:
	static ZFactoryChain_T* sHead_Preferred;
	static ZFactoryChain_T* sHead_Normal;
	ZFactoryChain_T* fNext;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZFactoryChain_Maker_T

template <class Result, class Param>
class ZFactoryChain_Maker_T : public ZFactoryChain_T<Result, Param>
	{
public:
	typedef bool (*Maker_t)(Result& oResult, Param iParam);

	ZFactoryChain_Maker_T(Maker_t iMaker)
	:	ZFactoryChain_T<Result, Param>(false),
		fMaker(iMaker)
		{}

	ZFactoryChain_Maker_T(bool iPreferred, Maker_t iMaker)
	:	ZFactoryChain_T<Result, Param>(iPreferred),
		fMaker(iMaker)
		{}

	virtual bool Make(Result& oResult, Param iParam)
		{ return fMaker(oResult, iParam); }

private:
	Maker_t fMaker;
	};

NAMESPACE_ZOOLIB_END

#endif // __ZFactoryChain__
