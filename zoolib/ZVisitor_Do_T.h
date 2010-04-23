/* -------------------------------------------------------------------------------------------------
Copyright (c) 2010 Andrew Green
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

#ifndef __ZVisitor_Do_T__
#define __ZVisitor_Do_T__
#include "zconfig.h"

#include "zoolib/ZDebug.h"
#include "zoolib/ZSetRestore_T.h"
#include "zoolib/ZVisitor.h"

NAMESPACE_ZOOLIB_BEGIN

// =================================================================================================
#pragma mark -
#pragma mark * ZVisitor_Do_T

template <class Result_t>
class ZVisitor_Do_T
:	public virtual ZVisitor
	{
public:
	ZVisitor_Do_T()
	:	fResultPtr(nullptr)
		{}

// Our protocol
	bool Do(ZRef<ZVisitee> iRep, Result_t& oResult)
		{
		if (iRep)
			{
			ZSetRestore_T<Result_t*> sr(fResultPtr, &oResult);
			iRep->Accept(*this);
			return !fResultPtr;
			}
		return false;
		}

	Result_t Do(ZRef<ZVisitee> iRep)
		{
		Result_t result;
		this->Do(iRep, result);
		return result;
		}

protected:
	void pSetResult(const Result_t& iResult)
		{
		ZAssert(fResultPtr);
		*fResultPtr = iResult;
		fResultPtr = nullptr;
		}

private:
	Result_t* fResultPtr;
	};

NAMESPACE_ZOOLIB_END

#endif // __ZVisitor_Do_T__
