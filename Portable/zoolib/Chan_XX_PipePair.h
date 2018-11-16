/* -------------------------------------------------------------------------------------------------
Copyright (c) 2018 Andrew Green
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

#ifndef __ZooLib_Chan_XX_PipePair_h__
#define __ZooLib_Chan_XX_PipePair_h__ 1
#include "zconfig.h"

#include "zoolib/Chan.h"
#include "zoolib/Time.h"
#include "zoolib/ZCounted.h"
#include "zoolib/ZThread.h"

namespace ZooLib {

// =================================================================================================
#pragma mark - ImpPipePair

template <class EE>
class ImpPipePair
:	public ZCounted
	{
public:
	ImpPipePair()
		{
		fClosed = false;

		fSource = nullptr;
		fSourceEnd = nullptr;

		fDest = nullptr;
		fDestCount = 0;
		}

	~ImpPipePair()
		{
		ZAcqMtx acq(fMutex);
		ZAssertStop(2, fSource == nullptr && fDest == nullptr);
		}

// For Aspect_Abort
	void Abort()
		{
		ZAcqMtx acq(fMutex);
		if (not fClosed)
			{
			fClosed = true;
			fCondition_Read.Broadcast();
			fCondition_Write.Broadcast();
			}
		}

// For Aspect_DisconnectRead
	bool DisconnectRead(double iTimeout)
		{
		ZAcqMtx acq(fMutex);
		if (not fClosed)
			{
			fSource = fSourceEnd;
			fClosed = true;
			fCondition_Write.Broadcast();
			}
		return true;
		}

// For Aspect_DisconnectWrite
	void DisconnectWrite()
		{
		ZAcqMtx acq(fMutex);
		if (not fClosed)
			{
			fClosed = true;
			fCondition_Read.Broadcast();
			}
		}

// For Aspect_Read
	size_t Read(EE* oDest, size_t iCount)
		{
		EE* localDest = static_cast<EE*>(oDest);
		EE* localEnd = localDest + iCount;

		ZAcqMtx acq(fMutex);
		while (localDest < localEnd)
			{
			if (fSource && fSource < fSourceEnd)
				{
				// We've got a source waiting to give us data.
				size_t countToCopy = std::min(localEnd - localDest, fSourceEnd - fSource);
				std::copy(fSource, fSource + countToCopy, localDest);
				localDest += countToCopy;
				fSource += countToCopy;
				fCondition_Write.Broadcast();
				break;
				}
			else
				{
				// Register ourselves as waiting for data.
				if (fClosed)
					break;
				ZAssertStop(2, fDest == nullptr && fDestCount == 0);
				fDest = localDest;
				fDestCount = localEnd - localDest;
				fCondition_Write.Broadcast();
				fCondition_Read.Wait(fMutex);
				localDest = fDest;
				fDest = nullptr;
				fDestCount = 0;
				}
			}

		return localDest - static_cast<EE*>(oDest);
		}

	size_t Readable()
		{
		ZAcqMtx acq(fMutex);
		if (fSource)
			return fSourceEnd - fSource;
		return 0;
		}

// For Aspect_WaitReadable
	bool WaitReadable(double iTimeout)
		{
		const double deadline = Time::sSystem() + iTimeout;
		ZAcqMtx acq(fMutex);
		for (;;)
			{
			if ((fSource && fSource < fSourceEnd) || fClosed)
				return true;

			if (not fCondition_Read.WaitUntil(fMutex, deadline))
				return false;
			}
		}

// For Aspect_Write
	size_t Write(const EE* iSource, size_t iCount)
		{
		const EE* localSource = static_cast<const EE*>(iSource);
		const EE* localEnd = localSource + iCount;

		ZAcqMtx acq(fMutex);
		while (localSource < localEnd && not fClosed)
			{
			if (fDestCount)
				{
				// A reader is waiting for data, so copy straight
				// from our source into the reader's dest.
				size_t countToCopy = std::min(fDestCount, size_t(localEnd - localSource));
				std::copy(localSource, localSource + countToCopy, fDest);
				fDest += countToCopy;
				localSource += countToCopy;
				fDestCount -= countToCopy;
				fCondition_Read.Broadcast();
				break;
				}
			else
				{
				// Register ourselves as having data to provide.
				ZAssertStop(2, fSource == nullptr && fSourceEnd == nullptr);
				fSource = localSource;
				fSourceEnd = localEnd;
				fCondition_Read.Broadcast();
				fCondition_Write.Wait(fMutex);
				localSource = fSource;
				fSource = nullptr;
				fSourceEnd = nullptr;
				}
			}

		return localSource - static_cast<const EE*>(iSource);
		}

private:
	ZMtx fMutex;
	ZCnd fCondition_Read;
	ZCnd fCondition_Write;

	bool fClosed;

	const EE* fSource;
	const EE* fSourceEnd;

	EE* fDest;
	size_t fDestCount;
	};

// ----------

template <class EE>
class ChanR_PipePair
:	public ChanR<EE>
	{
public:
	ChanR_PipePair(const ZRef<ImpPipePair<EE>>& iPipePair)
	:	fPipePair(iPipePair)
		{}

	virtual ~ChanR_PipePair()
		{
		while (not fPipePair->DisconnectRead(1 * Time::kDay))
			{}
		}

// From Aspect_Read
	virtual size_t Read(EE* oDest, size_t iCount)
		{ return fPipePair->Read(oDest, iCount); }

	virtual size_t Readable()
		{ return fPipePair->Readable(); }

	ZRef<ImpPipePair<EE>> fPipePair;
	};

// ----------

template <class EE>
class ChanWCon_PipePair
:	public ChanWCon<EE>
	{
public:
	ChanWCon_PipePair(const ZRef<ImpPipePair<EE>>& iPipePair)
	:	fPipePair(iPipePair)
		{}

// From Aspect_Abort
	virtual void Abort()
		{ fPipePair->Abort(); }

// From Aspect_DisconnectWrite
	virtual void DisconnectWrite()
		{ return fPipePair->DisconnectWrite(); }

// From Aspect_Write
	virtual size_t Write(const EE* iSource, size_t iCount)
		{ return fPipePair->Write(iSource, iCount); }

	ZRef<ImpPipePair<EE>> fPipePair;
	};

} // namespace ZooLib

#endif // __ZooLib_Chan_XX_PipePair_h__
