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

#include "zoolib/zql/ZQL_Util_Strim_RelHead.h"
#include "zoolib/ZYad_ZooLibStrim.h"

NAMESPACE_ZOOLIB_BEGIN
namespace ZQL {
namespace Util_Strim_RelHead {

// =================================================================================================
#pragma mark -
#pragma mark * Util_Strim_RelHead

using std::set;
using std::string;

void sWrite_PropName(const string& iName, const ZStrimW& s)
	{
	s.Write("@");
	ZYad_ZooLibStrim::sWrite_PropName(iName, s);
	}

void sWrite_RelHead(const RelHead& iRelHead, const ZStrimW& s)
	{
	bool universal;
	const set<string>& names = iRelHead.GetElems(universal);

	s.Write("[");
	if (universal)
		s.Write("~");

	bool isFirst = true;
	for (set<string>::const_iterator i = names.begin(); i != names.end(); ++i)
		{
		if (!isFirst)
			s.Write(", ");
		isFirst = false;
		sWrite_PropName(*i, s);
		}
	s.Write("]");
	}

} // namespace Util_Strim_RelHead
} // namespace ZQL
NAMESPACE_ZOOLIB_END
