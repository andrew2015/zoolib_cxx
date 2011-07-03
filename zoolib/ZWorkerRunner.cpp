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

#include "zoolib/ZWorkerRunner.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark * ZWorkerRunner

bool ZWorkerRunner::pAttachWorker(ZRef<ZWorker> iWorker)
	{
	ZAssert(iWorker);
	ZAssert(not iWorker->fRunner.Get());

	iWorker->fRunner = MakeRef(this);

	try
		{
		if (ZRef<ZWorker::Callable_Attached_t> theCallable = iWorker->fCallable_Attached)
			theCallable->Call(iWorker);

		try
			{
			iWorker->RunnerAttached();
			return true;
			}
		catch (...)
			{}

		try
			{
			if (ZRef<ZWorker::Callable_Detached_t> theCallable = iWorker->fCallable_Detached)
				theCallable->Call(iWorker);
			}
		catch (...)
			{}
		}
	catch (...)
		{}

	iWorker->fRunner.Clear();

	return false;
	}

void ZWorkerRunner::pDetachWorker(ZRef<ZWorker> iWorker)
	{
	ZAssert(iWorker);
	ZAssert(iWorker->fRunner.Get() == this);

	iWorker->fRunner.Clear();

	try { iWorker->RunnerDetached(); }
	catch (...) {}

	try
		{
		if (ZRef<ZWorker::Callable_Detached_t> theCallable = iWorker->fCallable_Detached)
			theCallable->Call(iWorker);
		}
	catch (...)
		{}
	}

bool ZWorkerRunner::pInvokeWork(ZRef<ZWorker> iWorker)
	{
	try { return iWorker->Work(); }
	catch (...) {}

	return false;
	}

} // namespace ZooLib
