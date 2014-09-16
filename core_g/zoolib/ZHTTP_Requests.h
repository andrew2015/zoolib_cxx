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

#ifndef __ZHTTP_Requests_h__
#define __ZHTTP_Requests_h__ 1
#include "zconfig.h"

#include "zoolib/Callable.h"

#include "zoolib/ZHTTP.h"

namespace ZooLib {
namespace ZHTTP {

// =================================================================================================
// MARK: - ZHTTP

typedef Callable<ZRef<ZStreamerRW>(const string& iHost, uint16 iPort, bool iUseSSL)>
	Callable_Connect;

ZRef<ZStreamerRW> sStreamerRW(const string& iHost, uint16 iPort, bool iUseSSL);

// -----

ZRef<ZStreamerR> sRequest(const ZRef<Callable_Connect>& iCallable_Connect,
	const string& iMethod, const string& iURL, const Map* iHeader,
	bool iConnectionClose,
	string* oURL, int32* oResponseCode, Map* oHeader, Data* oRawHeader);

// -----

ZRef<ZStreamerR> sPOST_Send(ZRef<Callable_Connect> iCallable_Connect,
	const string& iMethod,
	const string& iURL, const Map* iHeader, const ZStreamR& iBody);

ZRef<ZStreamerR> sPOST_Receive(const ZRef<ZStreamerR>& iStreamerR,
	int32* oResponseCode, Map* oHeader, Data* oRawHeader);

ZRef<ZStreamerR> sPOST(ZRef<Callable_Connect> iCallable_Connect,
	const string& iURL, const Map* iHeader, const ZStreamR& iBody,
	int32* oResponseCode, Map* oHeader, Data* oRawHeader);

// -----

bool sCONNECT(const ZStreamR& r, const ZStreamW& w,
	const string& iAddress, const Map* iHeader,
	int32* oResponseCode, Map* oHeader);

} // namespace ZHTTP
} // namespace ZooLib

#endif // __ZHTTP_Requests_h__