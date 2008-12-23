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

#ifndef __ZStreamCopier__
#define __ZStreamCopier__
#include "zconfig.h"

#include "zoolib/ZStreamReader.h"

NAMESPACE_ZOOLIB_BEGIN

// =================================================================================================
#pragma mark -
#pragma mark * ZStreamCopier

class ZStreamCopier : public ZStreamReader
	{
public:
	ZStreamCopier(ZRef<ZStreamerW> iStreamerW);
	ZStreamCopier(ZRef<ZStreamerW> iStreamerW, size_t iChunkSize);

	~ZStreamCopier();

// From ZStreamReader
	virtual bool Read(const ZStreamR& iStreamR);

	virtual void RunnerDetached(ZStreamReaderRunner* iRunner);

private:
	ZRef<ZStreamerW> fStreamerW;
	size_t fChunkSize;
	};

NAMESPACE_ZOOLIB_END

#endif // __ZStreamCopier__
