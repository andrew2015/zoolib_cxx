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

#ifndef __ZUtil_Expr_Bool_CNF_h__
#define __ZUtil_Expr_Bool_CNF_h__
#include "zconfig.h"

#include "zoolib/TagVal.h"

#include "zoolib/ZExpr_Bool.h"

#include <set>

namespace ZooLib {
namespace Util_Expr_Bool {

// =================================================================================================
// MARK: - Util_Expr_Bool

// CNF == Conjunctive Normal Form
// Clause is a disjunction of literals

typedef std::set<TagVal<ZRef<ZExpr_Bool>,struct Tag_Clause> > Clause;
typedef std::set<Clause> CNF;

ZRef<ZExpr_Bool> sFromCNF(const CNF& iCNF);
CNF sAsCNF(const ZRef<ZExpr_Bool>& iExpr);

} // namespace Util_Expr_Bool
} // namespace ZooLib

#endif // __ZUtil_Expr_Bool_CNF_h__