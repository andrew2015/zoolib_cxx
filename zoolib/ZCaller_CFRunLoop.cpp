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

#include "zoolib/ZCaller_CFRunLoop.h"

#if ZCONFIG_API_Enabled(Caller_CFRunLoop)

#include "zoolib/ZUtil_CF_Context.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark * ZCaller_CFRunLoop

// Usable pre 10.5
static CFRunLoopRef spRunLoopMain = ::CFRunLoopGetCurrent();

static ZRef<ZCaller_CFRunLoop> spCaller;

ZRef<ZCaller_CFRunLoop> ZCaller_CFRunLoop::sMain()
	{
	if (!spCaller)
		{
		ZRef<ZCaller_CFRunLoop> theCaller = new ZCaller_CFRunLoop(::CFRunLoopGetMain());
		spCaller.AtomicSetIfNull(theCaller.Get());
		}
	return spCaller;
	}

ZCaller_CFRunLoop::ZCaller_CFRunLoop(CFRunLoopRef iRunLoop)
:	fRunLoop(iRunLoop)
	{}

ZCaller_CFRunLoop::~ZCaller_CFRunLoop()
	{}

void ZCaller_CFRunLoop::Initialize()
	{
	ZCaller::Initialize();

	fObserver = Adopt& ::CFRunLoopObserverCreate
		(nullptr, // allocator
		kCFRunLoopBeforeTimers, // activities
		true, // repeats
		0, // order
		spCallback,
		ZUtil_CF::Context<CFRunLoopObserverContext>(this->GetWeakRefProxy()).IParam());

	::CFRunLoopAddObserver(fRunLoop, fObserver, kCFRunLoopCommonModes);
	}

void ZCaller_CFRunLoop::Finalize()
	{
	::CFRunLoopObserverInvalidate(fObserver);

	ZCaller::Finalize();
	}

void ZCaller_CFRunLoop::pTrigger()
	{ ::CFRunLoopWakeUp(fRunLoop); }

void ZCaller_CFRunLoop::spCallback
	(CFRunLoopObserverRef observer, CFRunLoopActivity activity, void* info)
	{
	if (ZRef<ZCaller_CFRunLoop> theCaller =
		ZWeakRef<ZCaller_CFRunLoop>(static_cast<WeakRefProxy*>(info)))
		{ theCaller->pCall(); }
	}

} // namespace ZooLib

#endif // ZCONFIG_API_Enabled(Caller_CFRunLoop)