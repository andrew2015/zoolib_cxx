/* -------------------------------------------------------------------------------------------------
Copyright (c) 2009 Andrew Green
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

#include "zoolib/ZStreamerListener.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark * ZStreamerListener

ZStreamerListener::ZStreamerListener(ZRef<ZStreamerRWFactory> iFactory)
:	fFactory(iFactory)
	{
	ZAssert(fFactory);
	}

ZStreamerListener::~ZStreamerListener()
	{}

void ZStreamerListener::RunnerAttached()
	{
	ZWorker::RunnerAttached();
	this->ListenStarted();
	}

void ZStreamerListener::RunnerDetached()
	{
	this->ListenFinished();
	ZWorker::RunnerDetached();
	}

bool ZStreamerListener::Work()
	{
	if (ZRef<ZStreamerRWFactory> theFactory = fFactory)
		{
		if (this->Connected(theFactory->MakeStreamerRW()))
			{
			// ##
			// See comment in ZStreamerReader::Work.
			ZWorker::Wake();
			return true;
			}
		}
	return false;
	}

void ZStreamerListener::ListenStarted()
	{}

void ZStreamerListener::ListenFinished()
	{}

ZRef<ZStreamerRWFactory> ZStreamerListener::GetFactory()
	{ return fFactory; }

void ZStreamerListener::Stop()
	{
	if (ZRef<ZStreamerRWFactory> theFactory = fFactory)
		{
		fFactory.Clear();
		theFactory->Cancel();
		}
	ZWorker::Wake();
	}

} // namespace ZooLib
