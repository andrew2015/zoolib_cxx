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

#ifndef __ZFuture__
#define __ZFuture__ 1
#include "zconfig.h"

#include "zoolib/ZCountedWithoutFinalize.h"
#include "zoolib/ZQ.h"
#include "zoolib/ZThread.h"

namespace ZooLib {

template <class T> class ZPromise;

// =================================================================================================
#pragma mark -
#pragma mark * ZFuture

template <class T>
class ZFuture
:	public ZCountedWithoutFinalize
	{
	ZFuture()
	:	fPromiseExists(true)
		{}

public:
	virtual ~ZFuture()
		{}

	bool Wait()
		{
		ZAcqMtx acq(fMtx);
		while (fPromiseExists && !fVal)
			fCnd.Wait(fMtx);
		return fVal;
		}

	bool WaitFor(double iTimeout)
		{
		ZAcqMtx acq(fMtx);
		if (fPromiseExists && !fVal)
			fCnd.WaitFor(fMtx, iTimeout);
		return fVal;
		}

	bool WaitUntil(ZTime iDeadline)
		{
		ZAcqMtx acq(fMtx);
		if (fPromiseExists && !fVal)
			fCnd.WaitUntil(fMtx, iDeadline);
		return fVal;
		}

	ZQ<T> Get()
		{
		ZAcqMtx acq(fMtx);
		while (fPromiseExists && !fVal)
			fCnd.Wait(fMtx);
		return fVal;
		}

private:
	friend class ZPromise<T>;
	ZMtx fMtx;
	ZCnd fCnd;
	bool fPromiseExists;
	ZQ<T> fVal;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZFuture<void>

template <>
class ZFuture<void>
:	public ZCountedWithoutFinalize
	{
	ZFuture()
	:	fPromiseExists(true)
	,	fSet(false)
		{}

public:
	virtual ~ZFuture()
		{}

	bool Wait()
		{
		ZAcqMtx acq(fMtx);
		while (fPromiseExists && !fSet)
			fCnd.Wait(fMtx);
		return fSet;
		}

	bool WaitFor(double iTimeout)
		{
		ZAcqMtx acq(fMtx);
		if (fPromiseExists && !fSet)
			fCnd.WaitFor(fMtx, iTimeout);
		return fSet;
		}

	bool WaitUntil(ZTime iDeadline)
		{
		ZAcqMtx acq(fMtx);
		if (fPromiseExists && !fSet)
			fCnd.WaitUntil(fMtx, iDeadline);
		return fSet;
		}

private:
	friend class ZPromise<void>;
	ZMtx fMtx;
	ZCnd fCnd;
	bool fPromiseExists;
	bool fSet;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZPromise

template <class T>
class ZPromise
:	public ZCountedWithoutFinalize
	{
public:
	ZPromise()
	:	fFuture(new ZFuture<T>)
		{}

	virtual ~ZPromise()
		{
		ZAcqMtx acq(fFuture->fMtx);
		fFuture->fPromiseExists = false;
		fFuture->fCnd.Broadcast();
		}

	void Set(const T& iVal)
		{
		ZAcqMtx acq(fFuture->fMtx);
		fFuture->fVal = iVal;
		fFuture->fCnd.Broadcast();
		}

	ZRef<ZFuture<T> > Get()
		{ return fFuture; }

private:
	ZRef<ZFuture<T> > fFuture;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZPromise<void>

template <>
class ZPromise<void>
:	public ZCountedWithoutFinalize
	{
public:
	ZPromise()
	:	fFuture(new ZFuture<void>)
		{}

	virtual ~ZPromise()
		{
		ZAcqMtx acq(fFuture->fMtx);
		fFuture->fPromiseExists = false;
		fFuture->fCnd.Broadcast();
		}

	void Set()
		{
		ZAcqMtx acq(fFuture->fMtx);
		fFuture->fSet = true;
		fFuture->fCnd.Broadcast();
		}

	ZRef<ZFuture<void> > Get()
		{ return fFuture; }

private:
	ZRef<ZFuture<void> > fFuture;
	};

} // namespace ZooLib

#endif // __ZFuture__