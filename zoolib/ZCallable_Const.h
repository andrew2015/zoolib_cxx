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

#ifndef __ZCallable_Const__
#define __ZCallable_Const__ 1
#include "zconfig.h"

#include "zoolib/ZCallable.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark * ZCallable_Const

template <class R>
class ZCallable_Const
:	public ZCallable<R()>
	{
public:
	typedef typename ZCallableUtil::VT<R>::P Param_t;
	typedef typename ZCallableUtil::VT<R>::F Field_t;

	ZCallable_Const(Param_t iR)
	:	fR(iR)
		{}

// From ZCallable
	virtual R Call()
		{ return fR; }

private:
	Field_t fR;
	};

// =================================================================================================
#pragma mark -
#pragma mark * MakeCallable_Const

template <class R>
ZRef<ZCallable<R()> > MakeCallable_Const(R iR)
	{ return new ZCallable_Const<R>(iR); }

} // namespace ZooLib

#endif // __ZCallable_Const__