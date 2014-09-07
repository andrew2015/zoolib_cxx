/* -------------------------------------------------------------------------------------------------
Copyright (c) 2004 Andrew Green and Learning in Motion, Inc.
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

#ifndef __ZStreamRW_FlushOnRead_h__
#define __ZStreamRW_FlushOnRead_h__ 1
#include "zconfig.h"

#include "zoolib/ZStreamer.h"
#include "zoolib/ZThread.h"

namespace ZooLib {

// =================================================================================================
// MARK: - ZStreamRW_FlushOnRead

/// A RW stream that ensures the write stream is flushed when the read stream is read.

class ZStreamRW_FlushOnRead : public ZStreamR, public ZStreamW
	{
public:
	ZStreamRW_FlushOnRead(const ZStreamR& iStreamR, const ZStreamW& iStreamW);
	~ZStreamRW_FlushOnRead();

// From ZStreamR
	virtual void Imp_Read(void* oDest, size_t iCount, size_t* oCountRead);
	virtual size_t Imp_CountReadable();
	virtual bool Imp_WaitReadable(double iTimeout);

	virtual void Imp_CopyToDispatch(const ZStreamW& iStreamW, uint64 iCount,
		uint64* oCountRead, uint64* oCountWritten);

	virtual void Imp_CopyTo(const ZStreamW& iStreamW, uint64 iCount,
		uint64* oCountRead, uint64* oCountWritten);

	virtual void Imp_Skip(uint64 iCount, uint64* oCountSkipped);

// From ZStreamW
	virtual void Imp_Write(const void* iSource, size_t iCount, size_t* oCountWritten);

	virtual void Imp_CopyFromDispatch(const ZStreamR& iStreamR, uint64 iCount,
		uint64* oCountRead, uint64* oCountWritten);

	virtual void Imp_CopyFrom(const ZStreamR& iStreamR, uint64 iCount,
		uint64* oCountRead, uint64* oCountWritten);

	virtual void Imp_Flush();

protected:
	ThreadSafe_t fLastWasWrite;
	ZMtx fMutex;
	const ZStreamR& fStreamR;
	const ZStreamW& fStreamW;
	};

// =================================================================================================
// MARK: - ZStreamerRW_FlushOnRead

/// A RW streamer encapsulating a ZStreamRW_FlushOnRead.

class ZStreamerRW_FlushOnRead : public ZStreamerRW
	{
public:
	ZStreamerRW_FlushOnRead(ZRef<ZStreamerR> iStreamerR, ZRef<ZStreamerW> iStreamerW);
	virtual ~ZStreamerRW_FlushOnRead();

// From ZStreamerR via ZStreamerRW
	virtual const ZStreamR& GetStreamR();

// From ZStreamerW via ZStreamerRW
	virtual const ZStreamW& GetStreamW();

protected:
	ZRef<ZStreamerR> fStreamerR;
	ZRef<ZStreamerW> fStreamerW;
	ZStreamRW_FlushOnRead fStream;
	};

} // namespace ZooLib

#endif // __ZStreamRW_FlushOnRead_h__