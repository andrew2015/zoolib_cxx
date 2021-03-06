/* -------------------------------------------------------------------------------------------------
Copyright (c) 2013 Andrew Green
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

#ifndef __ZooLib_SocketWatcher_h__
#define __ZooLib_SocketWatcher_h__ 1
#include "zconfig.h"

#include "zoolib/Callable.h"
#include "zoolib/Compat_NonCopyable.h"

#include "zoolib/ZThread.h"

#include <set>

namespace ZooLib {

// =================================================================================================
#pragma mark - SocketWatcher

class SocketWatcher
:	NonCopyable
	{
public:
	SocketWatcher();

	typedef std::pair<int,ZP<Callable_Void>> Pair_t;

// -----

	bool QInsert(const Pair_t& iPair);
	bool QErase(const Pair_t& iPair);

	bool QInsert(int iSocket, const ZP<Callable_Void>& iCallable);
	bool QErase(int iSocket, const ZP<Callable_Void>& iCallable);

// -----

private:
	void pRun();
	static void spRun(SocketWatcher* iSocketWatcher);

	ZMtx fMtx;
	ZCnd fCnd;

	bool fThreadRunning;
	typedef std::set<Pair_t> Set_t;
	Set_t fSet;
	};

} // namespace ZooLib

#endif // __ZooLib_SocketWatcher_h__
