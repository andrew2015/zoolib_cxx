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

#ifndef __ZooLib_Chan_Bin_Stream_h__
#define __ZooLib_Chan_Bin_Stream_h__ 1
#include "zconfig.h"

#include "zoolib/ChanR_Bin.h"
#include "zoolib/ChanW_Bin.h"
#include "zoolib/ZStream.h"

namespace ZooLib {

// =================================================================================================
// MARK: - ChanR_Bin_Stream

class ChanR_Bin_Stream
:	public ChanR_Bin
	{
public:
	ChanR_Bin_Stream(const ZStreamR& iStreamR)
	:	fStreamR(iStreamR)
		{}

// From ChanR
	virtual size_t Read(Elmt* oDest, size_t iCount)
		{
		size_t countRead = 0;
		fStreamR.Read(oDest, iCount, &countRead);
		return countRead;
		}

	virtual uint64 Skip(uint64 iCount)
		{
		uint64 countSkipped;
		fStreamR.Skip(iCount, &countSkipped);
		return countSkipped;
		}

	virtual size_t Readable()
		{ return fStreamR.CountReadable(); }

private:
	const ZStreamR& fStreamR;
	};

// =================================================================================================
// MARK: - ChanW_Bin_Stream

class ChanW_Bin_Stream
:	public ChanW_Bin
	{
public:
	ChanW_Bin_Stream(const ZStreamW& iStreamW)
	:	fStreamW(iStreamW)
		{}

// From ChanW
	virtual size_t Write(const Elmt* iSource, size_t iCount)
		{
		size_t countWritten = 0;
		fStreamW.Write(iSource, iCount, &countWritten);
		return countWritten;
		}

	virtual void Flush()
		{ fStreamW.Flush(); }

private:
	const ZStreamW& fStreamW;
	};

} // namespace ZooLib

#endif // __ZooLib_Chan_Bin_Stream_h__