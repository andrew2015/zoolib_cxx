/* -------------------------------------------------------------------------------------------------
Copyright (c) 2009 Andrew Green
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

#ifndef __ZVal_Any_h__
#define __ZVal_Any_h__ 1
#include "zconfig.h"

#include "zoolib/SafePtrStack.h"
#include "zoolib/Util_Relops.h"

#include "zoolib/ZCompat_unordered_map.h"
#include "zoolib/ZCounted.h"
#include "zoolib/ZData_Any.h"
#include "zoolib/ZCompare_string.h" // For FastComparator_String
#include "zoolib/ZName.h"
#include "zoolib/ZVal.h"
#include "zoolib/ZVal_T.h"

#include <map>
#include <vector>

namespace ZooLib {

class ZSeq_Any;
class ZMap_Any;

typedef ZVal_T<ZMap_Any,ZSeq_Any> ZVal_Any;

// =================================================================================================
// MARK: - ZSeq_Any

class ZSeq_Any
	{
	class Rep;

public:
	typedef std::vector<ZVal_Any> Vector_t;
	typedef ZVal_Any Val;

	ZSeq_Any();
	ZSeq_Any(const ZSeq_Any& iOther);
	~ZSeq_Any();
	ZSeq_Any& operator=(const ZSeq_Any& iOther);

	ZSeq_Any(const Vector_t& iOther);
	ZSeq_Any& operator=(const Vector_t& iOther);

	template <class Iterator>
	ZSeq_Any(Iterator begin, Iterator end);

	int Compare(const ZSeq_Any& iOther) const;

// ZSeq protocol
	size_t Count() const;

	void Clear();

	ZVal_Any* PMut(size_t iIndex);
	const ZVal_Any* PGet(size_t iIndex) const;
	const ZQ<ZVal_Any> QGet(size_t iIndex) const;
	const ZVal_Any DGet(const ZVal_Any& iDefault, size_t iIndex) const;
	const ZVal_Any& Get(size_t iIndex) const;

	template <class S>
	const S* PGet(size_t iIndex) const
		{
		if (const ZVal_Any* theVal = this->PGet(iIndex))
			return theVal->PGet<S>();
		return nullptr;
		}

	template <class S>
	const ZQ<S> QGet(size_t iIndex) const
		{
		if (const ZVal_Any* theVal = this->PGet(iIndex))
			return theVal->QGet<S>();
		return null;
		}

	template <class S>
	const S& DGet(const S& iDefault, size_t iIndex) const
		{
		if (const S* theVal = this->PGet<S>(iIndex))
			return *theVal;
		return iDefault;
		}

	template <class S>
	const S& Get(size_t iIndex) const
		{
		if (const S* theVal = this->PGet<S>(iIndex))
			return *theVal;
		return sDefault<S>();
		}

	template <class S>
	S* PMut(size_t iIndex)
		{
		if (ZVal_Any* theVal = this->PMut(iIndex))
			return theVal->PMut<S>();
		return nullptr;
		}

	ZSeq_Any& Set(size_t iIndex, const ZVal_Any& iVal);

	ZSeq_Any& Erase(size_t iIndex);

	ZSeq_Any& Insert(size_t iIndex, const ZVal_Any& iVal);

	ZSeq_Any& Append(const ZVal_Any& iVal);

// Our protocol
	ZVal_Any& Mut(size_t iIndex);

	template <class S>
	S& Mut(size_t iIndex)
		{ return this->Mut(iIndex).Mut<S>(); }

	template <class S>
	S& DMut(const S& iDefault, size_t iIndex)
		{ return this->Mut(iIndex).DMut(iDefault); }

// Operators and STL-style
	bool operator==(const ZSeq_Any& r) const
		{ return this->Compare(r) == 0; }

	bool operator<(const ZSeq_Any& r) const
		{ return this->Compare(r) < 0; }

	ZVal_Any& operator[](size_t iIndex);
	const ZVal_Any& operator[](size_t iIndex) const;

	typedef Vector_t::iterator iterator;
	iterator begin();
	iterator end();

	typedef Vector_t::const_iterator const_iterator;
	const_iterator begin() const;
	const_iterator end() const;

private:
	void pTouch();

	ZRef<Rep> fRep;
	};

template <> struct RelopsTraits_HasEQ<ZSeq_Any> : public RelopsTraits_Has {};
template <> struct RelopsTraits_HasLT<ZSeq_Any> : public RelopsTraits_Has {};

// =================================================================================================
// MARK: - ZSeq_Any::Rep

class ZSeq_Any::Rep
:	public CountedWithoutFinalize
	{
private:
	Rep();
	virtual ~Rep();

	Rep(const Vector_t& iVector);

	template <class Iterator>
	Rep(Iterator begin, Iterator end)
	:	fVector(begin, end)
		{}

	Vector_t fVector;
	friend class ZSeq_Any;
	};

// =================================================================================================
// MARK: - ZSeq_Any, inline templated constructor

template <class Iterator>
ZSeq_Any::ZSeq_Any(Iterator begin, Iterator end)
:	fRep(new Rep(begin, end))
	{}

// =================================================================================================
// MARK: - ZNameVal

typedef std::pair<ZName, ZVal_Any> ZNameVal;

// =================================================================================================
// MARK: - ZMap_Any

class ZMap_Any
	{
public:
	class Rep;
	typedef ZName Name_t;

#if 0 && ZCONFIG_SPI_Enabled(unordered_map)
	typedef unordered_map<Name_t, ZVal_Any> Map_t;
#else
	typedef std::map<Name_t, ZVal_Any> Map_t;
#endif

	typedef Map_t::iterator Index_t;
	typedef ZVal_Any Val_t;

	ZMap_Any();
	ZMap_Any(const ZMap_Any& iOther);
	~ZMap_Any();
	ZMap_Any& operator=(const ZMap_Any& iOther);

	ZMap_Any(const Map_t& iOther);
	ZMap_Any& operator=(Map_t& iOther);

	ZMap_Any(const ZNameVal& iNV);
	ZMap_Any(const char* iName, const char* iVal);
	ZMap_Any(const Name_t& iName, const ZVal_Any& iVal);

	int Compare(const ZMap_Any& iOther) const;

// ZMap protocol
	bool IsEmpty() const;

	void Clear();

	const ZVal_Any* PGet(const Name_t& iName) const;
	const ZVal_Any* PGet(const Index_t& iIndex) const;

	const ZQ<ZVal_Any> QGet(const Name_t& iName) const;
	const ZQ<ZVal_Any> QGet(const Index_t& iIndex) const;

	const ZVal_Any& DGet(const ZVal_Any& iDefault, const Name_t& iName) const;
	const ZVal_Any& DGet(const ZVal_Any& iDefault, const Index_t& iIndex) const;

	const ZVal_Any& Get(const Name_t& iName) const;
	const ZVal_Any& Get(const Index_t& iIndex) const;

	ZVal_Any* PMut(const Name_t& iName);
	ZVal_Any* PMut(const Index_t& iIndex);

	template <class S>
	const S* PGet(const Name_t& iName) const
		{
		if (const ZVal_Any* theVal = this->PGet(iName))
			return theVal->PGet<S>();
		return nullptr;
		}

	template <class S>
	const S* PGet(const Index_t& iIndex) const
		{
		if (const ZVal_Any* theVal = this->PGet(iIndex))
			return theVal->PGet<S>();
		return nullptr;
		}

	template <class S>
	const ZQ<S> QGet(const Name_t& iName) const
		{
		if (const ZVal_Any* theVal = this->PGet(iName))
			return theVal->QGet<S>();
		return null;
		}

	template <class S>
	const ZQ<S> QGet(const Index_t& iIndex) const
		{
		if (const ZVal_Any* theVal = this->PGet(iIndex))
			return theVal->QGet<S>();
		return null;
		}

	template <class S>
	const S& DGet(const S& iDefault, const Name_t& iName) const
		{
		if (const S* theVal = this->PGet<S>(iName))
			return *theVal;
		return iDefault;
		}

	template <class S>
	const S& DGet(const S& iDefault, const Index_t& iIndex) const
		{
		if (const S* theVal = this->PGet<S>(iIndex))
			return *theVal;
		return iDefault;
		}

	template <class S>
	const S& Get(const Name_t& iName) const
		{
		if (const S* theVal = this->PGet<S>(iName))
			return *theVal;
		return sDefault<S>();
		}

	template <class S>
	const S& Get(const Index_t& iIndex) const
		{
		if (const S* theVal = this->PGet<S>(iIndex))
			return *theVal;
		return sDefault<S>();
		}

	template <class S>
	S* PMut(const Name_t& iName)
		{
		if (ZVal_Any* theVal = this->PMut(iName))
			return theVal->PMut<S>();
		return nullptr;
		}

	template <class S>
	S* PMut(const Index_t& iIndex)
		{
		if (ZVal_Any* theVal = this->PMut(iIndex))
			return theVal->PMut<S>();
		return nullptr;
		}

	ZMap_Any& Set(const Name_t& iName, const ZVal_Any& iVal);
	ZMap_Any& Set(const Index_t& iIndex, const ZVal_Any& iVal);

	template <class S>
	ZMap_Any& Set(const Name_t& iName, const S& iVal)
		{ return this->Set(iName, ZVal_Any(iVal)); }

	template <class S>
	ZMap_Any& Set(const Index_t& iIndex, const S& iVal)
		{ return this->Set(iIndex, ZVal_Any(iVal)); }

	ZMap_Any& Erase(const Name_t& iName);
	ZMap_Any& Erase(const Index_t& iIndex);

// Our protocol
	ZVal_Any& Mut(const Name_t& iName);

	template <class S>
	S& Mut(const Name_t& iName)
		{ return this->Mut(iName).Mut<S>(); }

	template <class S>
	S& DMut(const S& iDefault, const Name_t& iName)
		{
		if (S* theVal = this->PMut<S>(iName))
			return *theVal;
		this->Set(iName, iDefault);
		return *this->PMut<S>(iName);
		}
	
	Index_t Begin() const;
	Index_t End() const;

	const Name_t& NameOf(const Index_t& iIndex) const;
	Index_t IndexOf(const Name_t& iName) const;

	Index_t IndexOf(const ZMap_Any& iOther, const Index_t& iOtherIndex) const;

	ZMap_Any& Set(const ZNameVal& iNV);

	bool operator==(const ZMap_Any& r) const
		{ return this->Compare(r) == 0; }

	bool operator<(const ZMap_Any& r) const
		{ return this->Compare(r) < 0; }

	ZVal_Any& operator[](const Name_t& iName);
	const ZVal_Any& operator[](const Name_t& iName) const;

	ZVal_Any& operator[](const Index_t& iIndex);
	const ZVal_Any& operator[](const Index_t& iIndex) const;

private:
	void pTouch();
	Map_t::iterator pTouch(const Index_t& iIndex);

	ZRef<Rep> fRep;
	};

template <> struct RelopsTraits_HasEQ<ZMap_Any> : public RelopsTraits_Has {};
template <> struct RelopsTraits_HasLT<ZMap_Any> : public RelopsTraits_Has {};

ZMap_Any operator*(const ZNameVal& iNV0, const ZNameVal& iNV1);

ZMap_Any& operator*=(ZMap_Any& ioMap, const ZNameVal& iNV);
ZMap_Any operator*(const ZMap_Any& iMap, const ZNameVal& iNV);

ZMap_Any operator*(const ZNameVal& iNV, const ZMap_Any& iMap);

ZMap_Any& operator*=(ZMap_Any& ioMap0, const ZMap_Any& iMap1);
ZMap_Any operator*(const ZMap_Any& iMap0, const ZMap_Any& iMap1);

// -----

inline const ZVal_Any* sPGet(const ZMap_Any& iMap, const ZMap_Any::Name_t& iName)
	{ return iMap.PGet(iName); }

inline const ZQ<ZVal_Any> sQGet(const ZMap_Any& iMap, const ZMap_Any::Name_t& iName)
	{ return iMap.QGet(iName); }

inline const ZVal_Any& sDGet(const ZVal_Any& iDefault, const ZMap_Any& iMap, const ZMap_Any::Name_t& iName)
	{ return iMap.DGet(iDefault, iName); }

inline const ZVal_Any& sGet(const ZMap_Any& iMap, const ZMap_Any::Name_t& iName)
	{ return iMap.Get(iName); }

inline ZVal_Any* sPMut(ZMap_Any& ioMap, const ZMap_Any::Name_t& iName)
	{ return ioMap.PMut(iName); }

inline ZVal_Any& sDMut(const ZVal_Any& iDefault, ZMap_Any& ioMap, const ZMap_Any::Name_t& iName)
	{ return ioMap.DMut(iDefault, iName); }

inline ZVal_Any& sMut(ZMap_Any& ioMap, const ZMap_Any::Name_t& iName)
	{ return ioMap.Mut(iName); }

// inline ZVal_Any& sSet(ZMap_Any& ioMap, const ZMap_Any::Name_t& iName, const ZVal_Any& iVal)
//	{ return ioMap.Set(iName, iVal); }

// -----

template <class S>
const S* sPGet(const ZMap_Any& iMap, const ZMap_Any::Name_t& iName)
	{ return iMap.PGet<S>(iName); }

template <class S>
const ZQ<S> sQGet(const ZMap_Any& iMap, const ZMap_Any::Name_t& iName)
	{ return iMap.QGet<S>(iName); }

template <class S>
const S& sDGet(const S& iDefault, const ZMap_Any& iMap, const ZMap_Any::Name_t& iName)
	{ return iMap.DGet<S>(iDefault, iName); }

template <class S>
const S& sGet(const ZMap_Any& iMap, const ZMap_Any::Name_t& iName)
	{ return iMap.Get<S>(iName); }

template <class S>
S* sPMut(ZMap_Any& ioMap, const ZMap_Any::Name_t& iName)
	{ return ioMap.PMut<S>(iName); }

template <class S>
S& sDMut(const S& iDefault, ZMap_Any& ioMap, const ZMap_Any::Name_t& iName)
	{ return ioMap.DMut<S>(iDefault, iName); }

template <class S>
S& sMut(ZMap_Any& ioMap, const ZMap_Any::Name_t& iName)
	{ return ioMap.Mut<S>(iName); }

template <class S>
S& sSet(ZMap_Any& ioMap, const ZMap_Any::Name_t& iName, const S& iVal)
	{ return ioMap.Set<S>(iName, iVal); }

// =================================================================================================
// MARK: - ZMap_Any::Rep

class SafePtrStackLink_Map_Any_Rep
:	public SafePtrStackLink<ZMap_Any::Rep,SafePtrStackLink_Map_Any_Rep>
	{};

class ZMap_Any::Rep
:	public ZCounted
,	public SafePtrStackLink_Map_Any_Rep
	{
public:
	virtual ~Rep();

private:
	Rep();

	Rep(const Map_t& iMap);

// From ZCounted
	virtual void Finalize();

// Our protocol
	static ZRef<Rep> sMake();
	static ZRef<Rep> sMake(const Map_t& iMap);

	Map_t fMap;
	friend class ZMap_Any;
	};

// =================================================================================================
// MARK: - 

ZMap_Any sAugmented(const ZMap_Any& iUnder, const ZMap_Any& iOver);

} // namespace ZooLib

#endif // __ZVal_Any_h__