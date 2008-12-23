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

#include "zoolib/ZGRgn.h"
#include "zoolib/ZFactoryChain.h"

NAMESPACE_ZOOLIB_USING

// =================================================================================================
#pragma mark -
#pragma mark * Factory functions

ZOOLIB_FACTORYCHAIN_HEAD(ZRef<ZGRgnRep>, const ZRect&);

static ZRef<ZGRgnRep> sMake(const ZRect& iBounds)
	{
	return ZFactoryChain_T<ZRef<ZGRgnRep>, const ZRect&>
		::sMake(iBounds);
	}

// =================================================================================================
#pragma mark -
#pragma mark * ZGRgn

ZGRgnRep::ZGRgnRep()
	{}

// =================================================================================================
#pragma mark -
#pragma mark * ZGRgn

ZGRgn::ZGRgn()
	{}
ZGRgn::~ZGRgn()
	{}

ZGRgn::ZGRgn(const ZGRgn& iOther)
:	fRep(iOther.fRep)
	{}

ZGRgn& ZGRgn::operator=(const ZGRgn& iOther)
	{
	fRep = iOther.fRep;
	return *this;
	}

ZGRgn::ZGRgn(const ZRef<ZGRgnRep>& iRep)
:	fRep(iRep)
	{}

ZGRgn& ZGRgn::operator=(const ZRef<ZGRgnRep>& iRep)
	{
	fRep = iRep;
	return *this;
	}

ZGRgn::ZGRgn(const ZRect& iRect)
:	fRep(sMake(iRect))
	{}

ZGRgn::ZGRgn(ZCoord iLeft, ZCoord iTop, ZCoord iRight, ZCoord iBottom)
:	fRep(sMake(ZRect(iLeft, iTop, iRight, iBottom)))
	{}

ZGRgn& ZGRgn::operator=(const ZRect& iRect)
	{
	fRep = sMake(iRect);
	return *this;
	}

ZRef<ZGRgnRep> ZGRgn::GetRep() const
	{ return fRep; }

size_t ZGRgn::Decompose(DecomposeProc iProc, void* iRefcon) const
	{
	if (fRep)
		return fRep->Decompose(iProc, iRefcon);
	return 0;
	}

void ZGRgn::Clear()
	{ fRep.Clear(); }

bool ZGRgn::IsEmpty() const
	{ return !fRep && fRep->IsEmpty(); }

ZRect ZGRgn::Bounds() const
	{
	if (fRep)
		return fRep->Bounds();
	return ZRect(0, 0);
	}

bool ZGRgn::IsSimpleRect() const
	{
	if (fRep)
		return fRep->IsSimpleRect();
	return true;
	}

bool ZGRgn::Contains(ZCoord iH, ZCoord iV) const
	{
	if (fRep)
		return fRep->Contains(iH, iV);
	return false;
	}

bool ZGRgn::Contains(const ZPoint& iPoint) const
	{
	if (fRep)
		return fRep->Contains(iPoint.h, iPoint.v);
	return false;
	}

bool ZGRgn::IsEqualTo(const ZGRgn& iOther) const
	{
	if (fRep)
		{
		if (iOther.fRep)
			fRep->IsEqualTo(iOther.fRep);
		return fRep->IsEmpty();
		}
	else if (iOther.fRep)
		{
		return iOther.fRep->IsEmpty();
		}
	return true;
	}

void ZGRgn::Outline()
	{ *this -= this->Insetted(1, 1); }

ZGRgn ZGRgn::Outlined() const
	{ return *this - this->Insetted(1, 1); }

void ZGRgn::Inset(ZCoord iH, ZCoord iV)
	{
	if (fRep)
		{
		if (fRep->GetRefCount() > 1)
			fRep = fRep->Insetted(iH, iV);
		else
			fRep->Inset(iH, iV);
		}
	}

ZGRgn ZGRgn::Insetted(ZCoord iH, ZCoord iV) const
	{
	if (fRep)
		return ZGRgn(fRep->Insetted(iH, iV));
	return *this;
	}

void ZGRgn::Offset(ZCoord iOffsetH, ZCoord iOffsetV)
	{
	if (iOffsetH || iOffsetV)
		{
		if (fRep)
			{
			if (fRep->GetRefCount() > 1)
				fRep = fRep->Offsetted(iOffsetH, iOffsetV);
			else
				fRep->Offset(iOffsetH, iOffsetV);
			}
		}
	}

ZGRgn ZGRgn::Offsetted(ZCoord iOffsetH, ZCoord iOffsetV) const
	{
	if (iOffsetH || iOffsetV)
		{
		if (fRep)
			ZGRgn(fRep->Offsetted(iOffsetH, iOffsetV));
		}
	return *this;
	}

void ZGRgn::Include(const ZRect& iRect)
	{
	if (!fRep)
		fRep = sMake(iRect);
	else if (fRep->GetRefCount() > 1)
		fRep = fRep->Including(iRect);
	else
		fRep->Include(iRect);
	}

ZGRgn ZGRgn::Including(const ZRect& iRect) const
	{
	if (!fRep)
		return ZGRgn(sMake(iRect));
	else
		return ZGRgn(fRep->Including(iRect));
	}

void ZGRgn::Intersect(const ZRect& iRect)
	{
	if (fRep)
		{
		if (fRep->GetRefCount() > 1)
			fRep = fRep->Intersecting(iRect);
		else
			fRep->Intersect(iRect);
		}
	}

ZGRgn ZGRgn::Intersecting(const ZRect& iRect) const
	{
	if (fRep)
		return ZGRgn(fRep->Intersecting(iRect));
	return *this;
	}

void ZGRgn::Exclude(const ZRect& iRect)
	{
	if (fRep)
		{
		if (fRep->GetRefCount() > 1)
			fRep = fRep->Excluding(iRect);
		else
			fRep->Exclude(iRect);
		}
	}

ZGRgn ZGRgn::Excluding(const ZRect& iRect) const
	{
	if (fRep)
		return ZGRgn(fRep->Excluding(iRect));
	return *this;
	}

void ZGRgn::Include(const ZGRgn& iOther)
	{
	if (!fRep)
		fRep = iOther.fRep;
	else if (iOther.fRep)
		{
		if (fRep->GetRefCount() > 1)
			fRep = fRep->Including(iOther.fRep);
		else
			fRep->Include(iOther.fRep);
		}
	}

ZGRgn ZGRgn::Including(const ZGRgn& iOther) const
	{
	if (!fRep)
		return iOther;
	else if (iOther.fRep)
		return ZGRgn(fRep->Including(iOther.fRep));
	else
		return *this;
	}

void ZGRgn::Intersect(const ZGRgn& iOther)
	{
	if (fRep && iOther.fRep)
		{
		if (fRep->GetRefCount() > 1)
			fRep = fRep->Intersecting(iOther.fRep);
		else
			fRep->Intersect(iOther.fRep);
		}
	}

ZGRgn ZGRgn::Intersecting(const ZGRgn& iOther) const
	{
	if (fRep && iOther.fRep)
		return ZGRgn(fRep->Intersecting(iOther.fRep));
	return ZGRgn();
	}

void ZGRgn::Exclude(const ZGRgn& iOther)
	{
	if (fRep && iOther.fRep)
		{
		if (fRep->GetRefCount() > 1)
			fRep = fRep->Excluding(iOther.fRep);
		else
			fRep->Exclude(iOther.fRep);
		}
	}

ZGRgn ZGRgn::Excluding(const ZGRgn& iOther) const
	{
	if (fRep && iOther.fRep)
		return ZGRgn(fRep->Excluding(iOther.fRep));
	return ZGRgn();
	}

void ZGRgn::Xor(const ZGRgn& iOther)
	{
	if (!fRep)
		{
		fRep = iOther.fRep;
		}
	else if (iOther.fRep)
		{
		if (fRep->GetRefCount() > 1)
			fRep = fRep->Xoring(iOther.fRep);
		else
			fRep->Xor(iOther.fRep);
		}
	}

ZGRgn ZGRgn::Xoring(const ZGRgn& iOther) const
	{
	if (!fRep)
		return iOther;
	else if (iOther.fRep)
		return ZGRgn(fRep->Xoring(iOther.fRep));
	else
		return *this;
	}
