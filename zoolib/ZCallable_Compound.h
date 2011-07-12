/* -------------------------------------------------------------------------------------------------
Copyright (c) 2011 Andrew Green
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

#ifndef __ZCallable_Compound__
#define __ZCallable_Compound__ 1
#include "zconfig.h"

#include "zoolib/ZCallable.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark * ZCallable_Apply

template <class A, class B, class C>
class ZCallable_Apply
:	public ZCallable<A(C)>
	{
public:
	ZCallable_Apply(const ZRef<ZCallable<A(B)> >& iApply, const ZRef<ZCallable<B(C)> >& iCallable)
	:	fApply(iApply)
	,	fCallable(iCallable)
		{}

// From ZCallable
	virtual A Call(C iC)
		{ return sCall(fApply, sCall(fCallable, iC)); }

private:
	const ZRef<ZCallable<A(B)> > fApply;
	const ZRef<ZCallable<B(C)> > fCallable;
	};

// =================================================================================================
#pragma mark -
#pragma mark * sCallable_Apply

template <class A, class B, class C>
ZRef<ZCallable<A(C)> >
sCallable_Apply(const ZRef<ZCallable<A(B)> >& iApply, const ZRef<ZCallable<B(C)> >& iCallable)
	{ return new ZCallable_Apply<A,B,C>(iApply, iCallable); }

template <class A, class B, class C>
ZRef<ZCallable<A(C)> >
sApply(const ZRef<ZCallable<A(B)> >& iApply, const ZRef<ZCallable<B(C)> >& iCallable)
	{ return new ZCallable_Apply<A,B,C>(iApply, iCallable); }

// =================================================================================================
#pragma mark -
#pragma mark * ZCallable_Comma

template <class R0, class R1>
class ZCallable_Comma
:	public ZCallable<R1(void)>
	{
public:
	ZCallable_Comma(const ZRef<ZCallable<R0(void)> >& i0, const ZRef<ZCallable<R1(void)> >& i1)
	:	f0(i0)
	,	f1(i1)
		{}

// From ZCallable
	virtual R1 Call()
		{
		sCall(f0);
		return sCall(f1);
		}

private:
	const ZRef<ZCallable<R0(void)> > f0;
	const ZRef<ZCallable<R1(void)> > f1;
	};

template <class R0, class R1>
ZRef<ZCallable<R1(void)> >
sCallable_Comma(const ZRef<ZCallable<R0(void)> >& i0, const ZRef<ZCallable<R1(void)> >& i1)
	{ return new ZCallable_Comma<R0,R1>(i0, i1); }

template <class R0, class R1>
ZRef<ZCallable<R1(void)> >
sComma(const ZRef<ZCallable<R0(void)> >& i0, const ZRef<ZCallable<R1(void)> >& i1)
	{ return sCallable_Comma(i0, i1); }

template <class R0, class R1>
ZRef<ZCallable<R1(void)> >
operator,(const ZRef<ZCallable<R0(void)> >& i0, const ZRef<ZCallable<R1(void)> >& i1)
	{ return sCallable_Comma(i0, i1); }

// =================================================================================================
#pragma mark -
#pragma mark * ZCallable_For

template <class R_Init, class R_Inc>
class ZCallable_For
:	public ZCallable_Void
	{
public:
	ZCallable_For(const ZRef<ZCallable<R_Init(void)> >& iInit,
		const ZRef<ZCallable_Bool>& iCondition,
		const ZRef<ZCallable<R_Inc(void)> >& iInc)
	:	fInit(iInit)
	,	fCondition(iCondition)
	,	fInc(iInc)
		{}

// From ZCallable
	virtual void Call()
		{
		for (sCall(fInit); sCall(fCondition); sCall(fInc))
			{}
		}

private:
	const ZRef<ZCallable<R_Init(void)> > fInit;
	const ZRef<ZCallable_Bool>& fCondition;
	const ZRef<ZCallable<R_Inc(void)> >& fInc;
	};

template <class R_Init, class R_Inc>
ZRef<ZCallable_Void> sCallable_For(const ZRef<ZCallable<R_Init(void)> >& iInit,
	const ZRef<ZCallable_Bool>& iCondition,
	const ZRef<ZCallable<R_Inc(void)> >& iInc)
	{ return new ZCallable_For<R_Init,R_Inc>(iInit, iCondition, iInc); }

template <class R_Init, class R_Inc>
ZRef<ZCallable_Void> sFor(const ZRef<ZCallable<R_Init(void)> >& iInit,
	const ZRef<ZCallable_Bool>& iCondition,
	const ZRef<ZCallable<R_Inc(void)> >& iInc)
	{ return new ZCallable_For<R_Init,R_Inc>(iInit, iCondition, iInc); }

// =================================================================================================
#pragma mark -
#pragma mark * ZCallable_If

template <class R>
class ZCallable_If
:	public ZCallable<R(void)>
	{
public:
	ZCallable_If(const ZRef<ZCallable_Bool>& iCondition,
		const ZRef<ZCallable<R(void)> >& i0, const ZRef<ZCallable<R(void)> >& i1)
	:	fCondition(iCondition)
	,	f0(i0)
	,	f1(i1)
		{}

// From ZCallable
	virtual R Call()
		{
		if (sCall(fCondition))
			return sCall(f0);
		else
			return sCall(f1);
		}

private:
	const ZRef<ZCallable_Bool> fCondition;
	const ZRef<ZCallable<R(void)> > f0;
	const ZRef<ZCallable<R(void)> > f1;
	};

template <class R>
ZRef<ZCallable<R(void)> > sCallable_If(const ZRef<ZCallable_Bool>& iCondition,
	const ZRef<ZCallable<R(void)> >& i0, const ZRef<ZCallable<R(void)> >& i1)
	{ return new ZCallable_If<R>(iCondition, i0, i1); }

template <class R>
ZRef<ZCallable<R(void)> > sIf(const ZRef<ZCallable_Bool>& iCondition,
	const ZRef<ZCallable<R(void)> >& i0, const ZRef<ZCallable<R(void)> >& i1)
	{ return new ZCallable_If<R>(iCondition, i0, i1); }

// =================================================================================================
#pragma mark -
#pragma mark * Makers

template<class R>
class ZCallable_Repeat
:	public ZCallable_Void
	{
public:
	ZCallable_Repeat(size_t iCount, const ZRef<ZCallable<R(void)> >& iCallable)
	:	fCount(iCount)
	,	fCallable(iCallable)
		{}

// From ZCallable
	virtual void Call()
		{
		if (fCallable)
			{
			for (size_t theCount = fCount; theCount--; /*no inc*/)
				fCallable->Call();
			}
		}

private:
	const size_t fCount;
	const ZRef<ZCallable<R(void)> > fCallable;
	};

template <class R>
ZRef<ZCallable_Void> sCallable_Repeat(size_t iCount, const ZRef<ZCallable<R(void)> >& iCallable)
	{ return new ZCallable_Repeat<R>(iCount, iCallable); }

template <class R>
ZRef<ZCallable_Void> sRepeat(size_t iCount, const ZRef<ZCallable<R(void)> >& iCallable)
	{ return new ZCallable_Repeat<R>(iCount, iCallable); }

// =================================================================================================
#pragma mark -
#pragma mark * Makers

template <class R>
class ZCallable_While
:	public ZCallable_Void
	{
public:
	ZCallable_While
		(const ZRef<ZCallable_Bool>& iCondition, const ZRef<ZCallable<R(void)> >& iCallable)
	:	fCondition(iCondition)
	,	fCallable(iCallable)
		{}

// From ZCallable
	virtual void Call()
		{
		while (sCall(fCondition))
			sCall(fCallable);
		}

private:
	const ZRef<ZCallable_Bool> fCondition;
	const ZRef<ZCallable<R(void)> > fCallable;
	};

template <class R>
ZRef<ZCallable_Void> sCallable_While
	(const ZRef<ZCallable_Bool>& iCondition, const ZRef<ZCallable<R(void)> >& iCallable)
	{ return new ZCallable_While<R>(iCondition, iCallable); }

template <class R>
ZRef<ZCallable_Void> sWhile
	(const ZRef<ZCallable_Bool>& iCondition, const ZRef<ZCallable<R(void)> >& iCallable)
	{ return new ZCallable_While<R>(iCondition, iCallable); }

inline
ZRef<ZCallable<void(void)> > sCallable_While(const ZRef<ZCallable_Bool>& iCallable)
	{ return new ZCallable_While<void>(iCallable, null); }

inline
ZRef<ZCallable<void(void)> > sWhile(const ZRef<ZCallable_Bool>& iCallable)
	{ return new ZCallable_While<void>(iCallable, null); }

} // namespace ZooLib

#endif // __ZCallable_Compound__