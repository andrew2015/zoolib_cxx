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

#ifndef __ZVisitor_Expr_Bool_ValPred_Any_Compare__
#define __ZVisitor_Expr_Bool_ValPred_Any_Compare__ 1
#include "zconfig.h"

#include "zoolib/ZExpr_Bool_ValPred_Any.h"
#include "zoolib/ZVisitor_Expr_Bool_Compare.h"

namespace ZooLib {
namespace Visitor_Expr_Bool_ValPred_Any_Compare {

// =================================================================================================
#pragma mark -
#pragma mark * Visitor_Expr_Bool_ValPred_Any_Compare::Comparer_Bootstrap

struct Comparer_Bootstrap
:	public virtual Visitor_Expr_Bool_Compare::Comparer_Bootstrap
,	public virtual ZVisitor_Expr_Bool_ValPred_T<ZVal_Any>
	{
public:
	virtual void Visit_Expr_Bool_ValPred(ZRef<ZExpr_Bool_ValPred_Any> iExpr);
	};

// =================================================================================================
#pragma mark -
#pragma mark * Visitor_Expr_Bool_ValPred_Any_Compare::Comparer

struct Comparer
:	public virtual Visitor_Expr_Bool_Compare::Comparer
,	public virtual ZVisitor_Expr_Bool_ValPred_T<ZVal_Any>
	{
	virtual void Visit_Expr_Bool_ValPred(ZRef<ZExpr_Bool_ValPred_Any> iRep);
	};

// =================================================================================================
#pragma mark -
#pragma mark * Visitor_Expr_Bool_ValPred_Any_Compare::Comparer_GT_ValPred

struct Comparer_GT_ValPred
:	public virtual Visitor_Expr_Bool_Compare::Comparer_GT_Or
,	public virtual ZVisitor_Expr_Bool_ValPred_T<ZVal_Any>
	{
	virtual void Visit_Expr_Bool_ValPred(ZRef<ZExpr_Bool_ValPred_Any>);
	};

// =================================================================================================
#pragma mark -
#pragma mark * Visitor_Expr_Bool_ValPred_Any_Compare::Comparer_ValPred

struct Comparer_ValPred
:	public virtual Visitor_Expr_Bool_Compare::Comparer_GT_Or
,	public virtual ZVisitor_Expr_Bool_ValPred_T<ZVal_Any>
	{
	ZRef<ZExpr_Bool_ValPred_Any> fExpr;
	Comparer_ValPred(Visitor_Expr_Bool_Compare::Comparer_Bootstrap* iBootstrap,
		ZRef<ZExpr_Bool_ValPred_Any> iExpr);

	virtual void Visit_Expr_Bool_ValPred(ZRef<ZExpr_Bool_ValPred_Any> iExpr);
	};

} // namespace Visitor_Expr_Bool_ValPred_Any_Compare
} // namespace ZooLib

#endif // __ZVisitor_Expr_Bool_ValPred_Any_Compare__
