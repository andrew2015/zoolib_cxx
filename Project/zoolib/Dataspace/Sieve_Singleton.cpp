/* -------------------------------------------------------------------------------------------------
Copyright (c) 2014 Andrew Green
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

#include "zoolib/Dataspace/Sieve_Singleton.h"

#include "zoolib/Callable_Bind.h"
#include "zoolib/Callable_Function.h"
#include "zoolib/Callable_PMF.h"
#include "zoolib/Log.h"

#include "zoolib/ZMACRO_foreach.h"

#include "zoolib/RelationalAlgebra/PseudoMap.h"
#include "zoolib/RelationalAlgebra/Util_Strim_Rel.h"
#include "zoolib/RelationalAlgebra/GetRelHead.h"

namespace ZooLib {
namespace Dataspace {

using namespace Util_STL;
using namespace ZooLib::RelationalAlgebra;

// =================================================================================================
#pragma mark - Sieve_Singleton

Sieve_Singleton::Sieve_Singleton(ZP<RelsWatcher::Callable_Register> iCallable_Register,
	const ZP<Callable_DatonUpdate>& iCallable_DatonUpdate,
	const ZP<Callable_Changed>& iCallable_Changed,
	const ZP<Expr_Rel>& iRel,
	const ColName& iDatonColName,
	const RelHead& iEditableRelHead)
:	fCallable_Register(iCallable_Register)
,	fCallable_DatonUpdate(iCallable_DatonUpdate)
,	fCallable_Changed(iCallable_Changed)
,	fRel(iRel)
,	fDatonColName(iDatonColName)
,	fEditableRelHead(iEditableRelHead)
	{
	if (sNotEmpty(fEditableRelHead) and not sContains(sGetRelHead(fRel), fDatonColName))
		{
		if (ZLOGF(w, eErr))
			w << "No daton column named '" << fDatonColName << "' in relation " << fRel;
		ZUnimplemented();
		}
	}

void Sieve_Singleton::Initialize()
	{
	Counted::Initialize();
	fResultHandler = new ResultHandler(sCallable(sWP(this), &Sieve_Singleton::pChanged));
	this->Activate();
	}

void Sieve_Singleton::Activate()
	{
	if (not fRegistration)
		fRegistration = sCall(fCallable_Register, fResultHandler, fRel);
	}

void Sieve_Singleton::Deactivate()
	{
	fRegistration.Clear();
	fResult.Clear();
	}

ZQ<bool> Sieve_Singleton::QExists()
	{
	if (fRegistration and fResult)
		return 0 != fResult->Count();
	return null;
	}

bool Sieve_Singleton::IsLoadedAndExists()
	{ return fRegistration && fResult && fResult->Count(); }

bool Sieve_Singleton::IsLoaded()
	{ return fRegistration && fResult; }

ZQ<Map_ZZ> Sieve_Singleton::QGetMap()
	{
	if (not fMapQ)
		{
		if (fRegistration && fResult && fResult->Count())
			{
			// Take basic value from what's in fMapInDaton.
			fMapQ = fMapInDaton;
			// Put anything that's in our Result under what's in fMapInDaton.
			const RelHead theRelHead = fResult->GetRelHead();
			const Val_DB* theVals = fResult->GetValsAt(0);
			foreachv (const string8& theName, theRelHead)
				{
				const Val_DB& theVal = *theVals++;
				if (not fMapQ->PGet(theName))
					fMapQ->Set(theName, theVal.As<Val_ZZ>());
				}
			}
		}

	return fMapQ;
	}

Map_ZZ Sieve_Singleton::GetMap()
	{
	ZAssert(this->IsLoadedAndExists());
	return *this->QGetMap();
	}

Val_ZZ Sieve_Singleton::Get(const string8& iName)
	{ return this->GetMap().Get(iName).As<Val_ZZ>(); }

void Sieve_Singleton::Set(const string8& iName, const Val_ZZ& iVal)
	{ this->Set(NameVal(iName, iVal)); }

void Sieve_Singleton::Set(const Map_ZZ& iMap)
	{
	ZAssert(this->IsLoadedAndExists());

	bool anyChange = false;
	for (Map_ZZ::Index_t iter = iMap.Begin(); iter != iMap.End(); ++iter)
		{
		const string8& theName = iMap.NameOf(iter);
		if (sContains(fEditableRelHead, theName))
			{
			const Val_ZZ& theVal = iMap.Get(iter);
			fMapInDaton.Set(theName, theVal);
			anyChange = true;
			}
		}

	if (not anyChange)
		return;

	const Daton newDaton = Dataspace::sAsDaton(fMapInDaton);
	if (fDaton == newDaton)
		return;

	fMapQ.Clear();

	fCallable_DatonUpdate->Call(&newDaton, 1, &fDaton, 1);

	fDaton = newDaton;

	fCallable_Changed->Call(this, false);
	}

void Sieve_Singleton::pChanged(const ZP<Counted>& iRegistration, const ZP<Result>& iResult)
	{
	const bool wasLoaded = fRegistration && fResult;
	fMapQ.Clear();
	fResult = iResult;
	fMapInDaton.Clear();
	if (iResult->Count() && sNotEmpty(fEditableRelHead))
		{
		PseudoMap_RelHead thePM(iResult->GetRelHead(), iResult->GetValsAt(0));
		if (ZQ<Daton> theDatonQ = thePM.QGet<Daton>(fDatonColName))
			{ fDaton = *theDatonQ; }
		else
			{
			if (ZLOGF(w, eErr))
				w << "fDatonColName: " << fDatonColName << "\n" << fRel;
			ZUnimplemented();
			}
		fMapInDaton = *Dataspace::sAsVal(fDaton).QGet<Map_ZZ>();
		}
	fCallable_Changed->Call(this, not wasLoaded);
	}

} // namespace Dataspace
} // namespace ZooLib
