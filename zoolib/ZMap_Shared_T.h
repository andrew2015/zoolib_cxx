/* -------------------------------------------------------------------------------------------------
Copyright (c) 2012 Andrew Green
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

#ifndef __ZMap_Shared_T_h__
#define __ZMap_Shared_T_h__ 1
#include "zconfig.h"

#include "zoolib/ZCountedVal.h"

namespace ZooLib {

// =================================================================================================
// MARK: - ZMap_Shared_T

template <class Map_p, class Val_p = typename Map_p::Val>
class ZMap_Shared_T
	{
public:
	typedef Map_p Map;
	typedef Val_p Val;

	ZMap_Shared_T()
		{}

	ZMap_Shared_T(const ZMap_Shared_T& iOther)
	:	fRep(iOther.fRep)
		{}

	~ZMap_Shared_T()
		{}

	ZMap_Shared_T& operator=(const ZMap_Shared_T& iOther)
		{
		fRep = iOther.fRep;
		return *this;
		}

// Our ctor protocol
	ZMap_Shared_T(const Map& iMap)
	:	fRep(new Rep(iMap))
		{}


// ZMap protocol
	ZQ<Val_p> QGet(const string8& iName) const
		{
		if (fRep)
			return fRep->Get().QGet(iName);
		return null;
		}

	Val_p DGet(const Val_p& iDefault, const string8& iName) const
		{
		if (ZQ<Val_p> theVal = this->QGet(iName))
			return *theVal;
		return iDefault;
		}

	Val_p Get(const string8& iName) const
		{
		if (ZQ<Val_p> theVal = this->QGet(iName))
			return *theVal;
		return Val_p();
		}

	template <class S>
	ZQ<S> QGet(const string8& iName) const
		{
		if (ZQ<Val_p> theQ = this->QGet(iName))
			return theQ->QGet<S>();
		return null;
		}

	template <class S>
	S DGet(const S& iDefault, const string8& iName) const
		{
		if (ZQ<Val_p> theQ = this->QGet(iName))
			{
			if (ZQ<S> theQ2 = theQ->QGet<S>())
				return *theQ2;
			}
		return iDefault;
		}

	template <class S>
	S Get(const string8& iName) const
		{
		if (ZQ<Val_p> theQ = this->QGet(iName))
			{
			if (ZQ<S> theQ2 = theQ->QGet<S>())
				return *theQ2;
			}
		return S();
		}

// Our protocol
	const Val_p operator[](const string8& iName) const
		{ return this->Get(iName); }

protected:
	typedef ZCountedVal<Map_p> Rep;
	ZRef<Rep> fRep;
	};

// =================================================================================================
// MARK: - ZMap_SharedMutable_T

template <class Map_p, class Val_p = typename Map_p::Val>
class ZMap_SharedMutable_T
:	public ZMap_Shared_T<Map_p,Val_p>
	{
	typedef ZMap_Shared_T<Map_p,Val_p> inherited;
	using inherited::fRep;

public:
	using inherited::Map;
	using inherited::Val;

	ZMap_SharedMutable_T()
		{}

	ZMap_SharedMutable_T(const ZMap_SharedMutable_T& iOther)
	:	inherited(iOther)
		{}

	~ZMap_SharedMutable_T()
		{}

	ZMap_SharedMutable_T& operator=(const ZMap_SharedMutable_T& iOther)
		{
		inherited::operator=(iOther);
		return *this;
		}

// Our ctor protocol
	ZMap_SharedMutable_T(const Map_p& iMap)
	:	inherited(iMap)
		{}

// Pick up stuff from inherited
	using inherited::operator[];

// ZMap mutable protocol
	void Clear()
		{
		if (fRep)
			fRep->GetMutable().Clear();
		}

	ZMap_SharedMutable_T& Set(const string8& iName, const Val_p& iVal)
		{
		if (not fRep)
			fRep = sCountedVal<Map_p>();
		fRep->GetMutable().Set(iName, iVal);
		return *this;
		}

	template <class S>
	ZMap_SharedMutable_T& Set(const string8& iName, const S& iVal)
		{ return this->Set(iName, Val_p(iVal)); }

	ZMap_SharedMutable_T& Erase(const string8& iName)
		{
		if (fRep)
			fRep->GetMutable().Erase(iName);
		return *this;
		}

// Our protocol
	Val_p& Mutable(const string8& iName)
		{
		if (not fRep)
			fRep = sCountedVal<Map_p>();
		return fRep->GetMutable().Mutable(iName);
		}

	Val_p& operator[](const string8& iName)
		{ return this->Mutable(iName); }
	};

} // namespace ZooLib

#endif // __ZMap_Shared_T_h__