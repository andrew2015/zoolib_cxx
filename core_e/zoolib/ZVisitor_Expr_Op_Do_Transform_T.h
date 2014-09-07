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

#ifndef __ZVisitor_Expr_Op_Do_Transform_T_h__
#define __ZVisitor_Expr_Op_Do_Transform_T_h__
#include "zconfig.h"

#include "zoolib/ZExpr_Op_T.h"
#include "zoolib/ZVisitor_Do_T.h"

namespace ZooLib {

// =================================================================================================
// MARK: - ZVisitor_Expr_Op_Do_Transform_T

template <class T>
class ZVisitor_Expr_Op_Do_Transform_T
:	public virtual ZVisitor_Do_T<ZRef<T> >
,	public virtual ZVisitor_Expr_Op0_T<T>
,	public virtual ZVisitor_Expr_Op1_T<T>
,	public virtual ZVisitor_Expr_Op2_T<T>
	{
public:
// From ZVisitor
	virtual void Visit(const ZRef<ZVisitee>& iRep)
		{ ZUnimplemented(); }

// From ZVisitor_Expr_Op0_T
	virtual void Visit_Expr_Op0(const ZRef<ZExpr_Op0_T<T> >& iExpr)
		{ this->pSetResult(iExpr->SelfOrClone()); }

// From ZVisitor_Expr_Op1_T
	virtual void Visit_Expr_Op1(const ZRef<ZExpr_Op1_T<T> >& iExpr)
		{
		if (ZQ<ZRef<T> > theQ0 = this->QDo(iExpr->GetOp0()))
			this->pSetResult(iExpr->SelfOrClone(*theQ0));
		}

// From ZVisitor_Expr_Op2_T
	virtual void Visit_Expr_Op2(const ZRef<ZExpr_Op2_T<T> >& iExpr)
		{
		if (ZQ<ZRef<T> > theQ0 = this->QDo(iExpr->GetOp0()))
			{
			if (ZQ<ZRef<T> > theQ1 = this->QDo(iExpr->GetOp1()))
				this->pSetResult(iExpr->SelfOrClone(*theQ0, *theQ1));
			}
		}
	};

} // namespace ZooLib

#endif // __ZVisitor_Expr_Op_Do_Transform_T_h__