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

#ifndef __ZooLib_PullPush_JSON_h__
#define __ZooLib_PullPush_JSON_h__ 1
#include "zconfig.h"

#include "zoolib/ChanRU_UTF.h"
#include "zoolib/PullPush.h"
#include "zoolib/Util_Chan_JSON.h" // For WriteOptions etc.

namespace ZooLib {
namespace PullPush_JSON {

// =================================================================================================
#pragma mark - 

using Util_Chan_JSON::ReadOptions;
using Util_Chan_JSON::WriteOptions;

} // namespace PullPush_JSON

// =================================================================================================
#pragma mark - 

bool sPull_JSON_Push(const ChanRU_UTF& iChanRU,
	const PullPush_JSON::ReadOptions& iRO,
	const ChanW_Any& iChanW);

bool sPull_Push_JSON(const ChanR_Any& iChanR, const ChanW_UTF& iChanW);

bool sPull_Push_JSON(const ChanR_Any& iChanR,
	size_t iInitialIndent, const PullPush_JSON::WriteOptions& iOptions,
	const ChanW_UTF& iChanW);

} // namespace ZooLib

#endif // __ZooLib_PullPush_JSON_h__
