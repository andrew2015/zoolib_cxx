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

#ifndef __ZooLib_Callable_Delay_h__
#define __ZooLib_Callable_Delay_h__ 1
#include "zconfig.h"

#include "zoolib/Callable.h"

#include "zoolib/ZThread.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark Callable_DelayFor

template <class R>
class Callable_DelayFor
:	public Callable<R(void)>
	{
public:
	Callable_DelayFor(double iInterval, const ZRef<Callable<R(void)> >& iCallable)
	:	fInterval(iInterval)
	,	fCallable(iCallable)
		{}

// From Callable
	virtual ZQ<R> QCall()
		{
		const ZTime systemTime = ZTime::sSystem() + fInterval;
		for (;;)
			{
			const double delta = systemTime - ZTime::sSystem();
			if (delta <= 0)
				return sQCall(fCallable);
			ZThread::sSleep(delta);
			}
		}

private:
	const double fInterval;
	const ZRef<Callable<R(void)> > fCallable;
	};

template <class R>
ZRef<Callable<R(void)> >
sCallable_DelayFor(double iInterval, const ZRef<Callable<R(void)> >& iCallable)
	{ return new Callable_DelayFor<R>(iInterval, iCallable); }

// =================================================================================================
#pragma mark -
#pragma mark Callable_DelayUntil

template <class R>
class Callable_DelayUntil
:	public Callable<R(void)>
	{
public:
	Callable_DelayUntil(ZTime iSystemTime, const ZRef<Callable<R(void)> >& iCallable)
	:	fSystemTime(iSystemTime)
	,	fCallable(iCallable)
		{}

// From Callable
	virtual ZQ<R> QCall()
		{
		for (;;)
			{
			const double delta = fSystemTime - ZTime::sSystem();
			if (delta <= 0)
				return sQCall(fCallable);
			ZThread::sSleep(delta);
			}
		}

private:
	const ZTime fSystemTime;
	const ZRef<Callable<R(void)> > fCallable;
	};

template <class R>
ZRef<Callable<R(void)> >
sCallable_DelayUntil(ZTime iSystemTime, const ZRef<Callable<R(void)> >& iCallable)
	{ return new Callable_DelayUntil<R>(iSystemTime, iCallable); }

} // namespace ZooLib

#endif // __ZooLib_Callable_Delay_h__