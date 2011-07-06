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

#ifndef __ZCallFuture__
#define __ZCallFuture__ 1
#include "zconfig.h"

#include "zoolib/ZCallable.h"
#include "zoolib/ZCallable_Bind.h"
#include "zoolib/ZCallable_Function.h"
#include "zoolib/ZFuture.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark * sCallFuture

template <class T>
void sCallFuture_T(ZRef<ZPromise<T> > iPromise, ZRef<ZCallable<T(void)> > iCallable)
	{
	if (iCallable)
		iPromise->Set(iCallable->Call());
	}

template <class T>
ZRef<ZFuture<T> > sCallFuture(ZRef<ZCaller> iCaller, ZRef<ZCallable<T(void)> > iCallable)
	{
	ZRef<ZPromise<T> > thePromise = new ZPromise<T>;
	if (iCaller)
		iCaller->Call(sBindR(sCallable(sCallFuture_T<T>), thePromise, iCallable));
	return thePromise->Get();
	}

inline
void sCallFutureVoid(ZRef<ZPromise<void> > iPromise, ZRef<ZCallable<void(void)> > iCallable)
	{
	if (iCallable)
		iCallable->Call();
	iPromise->Set();
	}

inline
ZRef<ZFuture<void> > sCallFuture(ZRef<ZCaller> iCaller, ZRef<ZCallable<void(void)> > iCallable)
	{
	ZRef<ZPromise<void> > thePromise = new ZPromise<void>;
	if (iCaller)
		iCaller->Call(sBindR(sCallable(sCallFutureVoid), thePromise, iCallable));
	return thePromise->Get();
	}

} // namespace ZooLib

#endif // __ZCallFuture__
