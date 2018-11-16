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

#ifndef __ZooLib_Callable_Starter_h__
#define __ZooLib_Callable_Starter_h__ 1
#include "zconfig.h"

#include "zoolib/CallByStarter.h"

namespace ZooLib {

// =================================================================================================
#pragma mark - Callable_Starter

template <class Signature> class Callable_Starter;

// =================================================================================================
#pragma mark - Callable_Starter (specialization for 0 params)

template <class R>
class Callable_Starter<R(void)>
:	public Callable<R(void)>
	{
public:
	typedef R (Signature)();

	Callable_Starter(const ZRef<Starter>& iStarter, const ZRef<Callable<Signature> >& iCallable)
	:	fStarter(iStarter)
	,	fCallable(iCallable)
		{}

// From Callable
	virtual ZQ<R> QCall()
		{ return sQCallByStarter(fStarter, fCallable)->QGet().Get(); }

private:
	const ZRef<Starter> fStarter;
	const ZRef<Callable<Signature> > fCallable;
	};

// =================================================================================================
#pragma mark - Callable_Starter variants

#define ZMACRO_Callable_Callable(X) \
\
template <class R, ZMACRO_Callable_Class_P##X> \
class Callable_Starter<R(ZMACRO_Callable_P##X)> \
:	public Callable<R(ZMACRO_Callable_P##X)> \
	{ \
public: \
	typedef R (Signature)(ZMACRO_Callable_P##X); \
\
	Callable_Starter(const ZRef<Starter>& iStarter, const ZRef<Callable<Signature> >& iCallable) \
	:	fStarter(iStarter) \
	,	fCallable(iCallable) \
		{} \
\
	virtual ZQ<R> QCall(ZMACRO_Callable_Pi##X) \
		{ \
		return sQCallByStarter<R,ZMACRO_Callable_P##X>( fStarter, fCallable, \
			ZMACRO_Callable_i##X)->QGet().Get(); \
		} \
\
private:\
	const ZRef<Starter> fStarter; \
	const ZRef<Callable<Signature> > fCallable;\
	};

ZMACRO_Callable_Callable(0)
ZMACRO_Callable_Callable(1)
ZMACRO_Callable_Callable(2)
ZMACRO_Callable_Callable(3)
ZMACRO_Callable_Callable(4)
ZMACRO_Callable_Callable(5)
ZMACRO_Callable_Callable(6)
ZMACRO_Callable_Callable(7)
ZMACRO_Callable_Callable(8)
ZMACRO_Callable_Callable(9)
ZMACRO_Callable_Callable(A)
ZMACRO_Callable_Callable(B)
ZMACRO_Callable_Callable(C)
ZMACRO_Callable_Callable(D)
ZMACRO_Callable_Callable(E)
ZMACRO_Callable_Callable(F)

#undef ZMACRO_Callable_Callable

// =================================================================================================
#pragma mark - sCallable_Starter

template <class Signature_p>
ZRef<Callable<Signature_p> >
sCallable_Starter(
	const ZRef<Starter>& iStarter, const ZRef<Callable<Signature_p> >& iCallable)
	{ return new Callable_Starter<Signature_p>(iStarter, iCallable); }

} // namespace ZooLib

#endif // __ZooLib_Callable_Starter_h__
