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

#ifndef __ZooLib_Chan_Bin_Data_h__
#define __ZooLib_Chan_Bin_Data_h__ 1
#include "zconfig.h"

#include "zoolib/ChanPos.h"
#include "zoolib/ChanSize.h"
#include "zoolib/ChanSizeSet.h"
#include "zoolib/ChanR_Bin.h"
#include "zoolib/ChanU.h"
#include "zoolib/ChanW_Bin.h"
#include "zoolib/Util_Chan.h" // For sCopyAll

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark ChanBase_Bin_Data

template <class Data_p>
class ChanBase_Bin_Data
:	public ChanR_Bin
,	public ChanPos
,	public ChanSize
	{
public:
	typedef Data_p Data;

	ChanBase_Bin_Data(Data* ioDataPtr)
	:	fDataPtr(ioDataPtr)
	,	fPosition(0)
		{}

// From ChanR
	virtual size_t QRead(Elmt_t* oDest, size_t iCount)
		{
		const size_t theSize = fDataPtr->GetSize();
		const size_t countToCopy = std::min<size_t>(iCount,
			theSize > fPosition ? theSize - fPosition : 0);
		fDataPtr->CopyTo(fPosition, oDest, countToCopy);
		fPosition += countToCopy;
		return countToCopy;
		}

	virtual size_t Readable()
		{
		const size_t theSize = fDataPtr->GetSize();
		return theSize >= fPosition ? theSize - fPosition : 0;
		}

// From ChanSize
	virtual uint64 Size()
		{ return fDataPtr->GetSize(); }

// From ChanPos
	virtual uint64 Pos()
		{ return fPosition; }

	virtual void SetPos(uint64 iPos)
		{ fPosition = iPos; }

protected:
	Data* fDataPtr;
	size_t fPosition;
	};

// =================================================================================================
#pragma mark -
#pragma mark ChanRPos_Bin_Data

template <class Data_p>
class ChanRPos_Bin_Data
:	public ChanBase_Bin_Data<Data_p>
,	public ChanU<byte>
	{
public:
	typedef Data_p Data;

	ChanRPos_Bin_Data()
	:	ChanBase_Bin_Data<Data_p>(&fData)
		{}

	ChanRPos_Bin_Data(Data iData)
	:	ChanBase_Bin_Data<Data_p>(&fData)
	,	fData(iData)
		{}

// From ChanU
	virtual size_t Unread(const Elmt_t* iSource, size_t iCount)
		{
		const size_t countToCopy = std::min(iCount, this->fPosition);
		this->fPosition -= countToCopy;

		// See Chan_XX_Memory for a note regarding bogus unreads.

		return countToCopy;
		}

	virtual size_t UnreadableLimit()
		{ return this->fPosition; }

	Data fData;
	};

// =================================================================================================
#pragma mark -
#pragma mark ChanRWPos_Bin_Data

template <class Data_p>
class ChanRWPos_Bin_Data
:	public ChanBase_Bin_Data<Data_p>
,	public ChanU<byte>
,	public ChanW_Bin
,	public ChanSizeSet
	{
public:
	typedef Data_p Data;

	ChanRWPos_Bin_Data(Data* ioData)
	:	ChanBase_Bin_Data<Data_p>(ioData)
		{}

// From ChanU
	virtual size_t Unread(const byte* iSource, size_t iCount)
		{
		const size_t countToCopy = std::min<size_t>(iCount, this->fPosition);

		this->fDataPtr->CopyFrom(this->fPosition - countToCopy, iSource, countToCopy);

		this->fPosition -= countToCopy;

		return countToCopy;
		}

	virtual size_t UnreadableLimit()
		{ return this->fPosition; }

// From ChanW
	virtual size_t QWrite(const byte* iSource, size_t iCount)
		{
		const size_t newPosition = this->fPosition + iCount;
		if (this->fDataPtr->GetSize() < newPosition)
			this->fDataPtr->SetSize(newPosition);

		this->fDataPtr->CopyFrom(this->fPosition, iSource, iCount);

		this->fPosition = newPosition;

		return iCount;
		}

// From ChanSizeSet
	virtual void SizeSet(uint64 iSize)
		{
		if (this->fPosition > iSize)
			this->fPosition = iSize;
		this->fDataPtr->SetSize(iSize);
		}
	};

// =================================================================================================
#pragma mark -
#pragma mark Data stream reading functions

template <class Data_p>
Data_p sReadAll_T(const ChanR_Bin& iChanR)
	{
	Data_p theData;
	sCopyAll(iChanR, ChanRWPos_Bin_Data<Data_p>(&theData));
	return theData;
	}

template <class Data_p>
Data_p sRead_T(const ChanR_Bin& iChanR, size_t iSize)
	{
	Data_p theData;
	sCopyFully(iChanR, ChanRWPos_Bin_Data<Data_p>(&theData), iSize);
	return theData;
	}

} // namespace ZooLib

#endif // __ZooLib_Chan_Bin_Data_h__