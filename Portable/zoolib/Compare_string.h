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

#ifndef __ZooLib_Compare_string_h__
#define __ZooLib_Compare_string_h__ 1
#include "zconfig.h"

#include "zoolib/Compare_T.h"
#include "zoolib/Compat_MSVCStaticLib.h"

#include <string>

ZMACRO_MSVCStaticLib_Reference(Compare_string)

namespace ZooLib {

template <>
inline int sCompare_T(const std::string& iL, const std::string& iR)
	{ return iL.compare(iR); }

// =================================================================================================
#pragma mark - FastComparator_String

struct FastComparator_String : public std::binary_function<std::string,std::string,bool>
	{
	bool operator()(const std::string& iLeft, const std::string& iRight) const;
	};

} // namespace ZooLib

#endif // __ZooLib_Compare_string_h__
