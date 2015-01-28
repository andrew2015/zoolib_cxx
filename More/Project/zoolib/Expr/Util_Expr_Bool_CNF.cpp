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

#include "zoolib/Visitor_Do_T.h"
#include "zoolib/ZMACRO_foreach.h"

#include "zoolib/Expr/Util_Expr_Bool_CNF.h"

using std::set;

namespace ZooLib {
namespace Util_Expr_Bool {

// =================================================================================================
#pragma mark -
#pragma mark Helpers

namespace { // anonymous

CNF spCrossMultiply(const CNF& iCNF0, const CNF& iCNF1)
	{
	CNF result;
	foreachi (iter0, iCNF0)
		{
		foreachi (iter1, iCNF1)
			{
			Clause theClause = *iter0;
			theClause.insert(iter1->begin(), iter1->end());
			result.insert(theClause);
			}
		}
	return result;
	}

CNF spTrue()
	{
	Clause theClause;
	theClause.insert(sTrue());
	CNF result;
	result.insert(theClause);
	return result;
	}

CNF spFalse()
	{
	Clause theClause;
	CNF result;
	result.insert(theClause);
	return result;
	}

bool spIsFalse(const CNF& iCNF)
	{
	if (iCNF.size() == 1 && iCNF.begin()->size() == 0)
		return true;
	return false;
	}

} // anonymous namespace

// =================================================================================================
#pragma mark -
#pragma mark Visitor_AsCNF (anonymous)

namespace { // anonymous

class Visitor_AsCNF
:	public virtual Visitor_Do_T<CNF>
,	public virtual Visitor_Expr_Bool_True
,	public virtual Visitor_Expr_Bool_False
,	public virtual Visitor_Expr_Bool_Not
,	public virtual Visitor_Expr_Bool_And
,	public virtual Visitor_Expr_Bool_Or
	{
public:
	Visitor_AsCNF()
	:	fNegating(false)
		{}

	virtual void Visit_Expr_Op0(const ZRef<Expr_Op0_T<Expr_Bool> >& iExpr)
		{
		Clause theClause;
		if (fNegating)
			theClause.insert(sNot(iExpr->Self()));
		else
			theClause.insert(iExpr->Self());

		CNF result;
		result.insert(theClause);
		this->pSetResult(result);
		}

	virtual void Visit_Expr_Bool_True(const ZRef<Expr_Bool_True>& iRep)
		{
		if (fNegating)
			this->pSetResult(spFalse());
		else
			this->pSetResult(spTrue());
		}

	virtual void Visit_Expr_Bool_False(const ZRef<Expr_Bool_False>& iRep)
		{
		if (fNegating)
			this->pSetResult(spTrue());
		else
			this->pSetResult(spFalse());
		}

	virtual void Visit_Expr_Bool_Not(const ZRef<Expr_Bool_Not>& iRep)
		{
		fNegating = !fNegating;
		Visitor_Expr_Bool_Not::Visit_Expr_Bool_Not(iRep);
		fNegating = !fNegating;
		}

	virtual void Visit_Expr_Bool_And(const ZRef<Expr_Bool_And>& iRep)
		{
		const CNF theCNF0 = this->Do(iRep->GetOp0());
		CNF theCNF1 = this->Do(iRep->GetOp1());
		if (fNegating)
			{
			this->pSetResult(spCrossMultiply(theCNF0, theCNF1));
			}
		else
			{
			if (spIsFalse(theCNF0) || spIsFalse(theCNF1))
				{
				this->pSetResult(spFalse());
				}
			else
				{
				theCNF1.insert(theCNF0.begin(), theCNF0.end());
				this->pSetResult(theCNF1);
				}
			}
		}

	virtual void Visit_Expr_Bool_Or(const ZRef<Expr_Bool_Or>& iRep)
		{
		const CNF theCNF0 = this->Do(iRep->GetOp0());
		CNF theCNF1 = this->Do(iRep->GetOp1());
		if (fNegating)
			{
			if (spIsFalse(theCNF0) || spIsFalse(theCNF1))
				{
				this->pSetResult(spFalse());
				}
			else
				{
				theCNF1.insert(theCNF0.begin(), theCNF0.end());
				this->pSetResult(theCNF1);
				}
			}
		else
			{
			this->pSetResult(spCrossMultiply(theCNF0, theCNF1));
			}
		}

protected:
	bool fNegating;
	};

} // anonymous namespace

// =================================================================================================
#pragma mark -
#pragma mark Util_Expr_Bool

static ZRef<Expr_Bool> spFromClause(
	const Clause& iClause,
	const ZRef<Expr_Bool>& iTrue,
	const ZRef<Expr_Bool>& iFalse)
	{
	ZRef<Expr_Bool> result = iFalse;
	foreachi (iter, iClause)
		{
		ZRef<Expr_Bool> theExpr = iter->Get();
		if (theExpr == iTrue)
			{
			return iTrue;
			}
		else if (theExpr && theExpr != iFalse)
			{
			if (result != iFalse)
				result |= theExpr;
			else
				result = theExpr;
			}
		}
	return result;
	}

ZRef<Expr_Bool> sFromCNF(const CNF& iCNF)
	{
	ZRef<Expr_Bool> theTrue = sTrue();
	ZRef<Expr_Bool> theFalse = sFalse();
	ZRef<Expr_Bool> result = theTrue;
	foreachi (iter, iCNF)
		{
		ZRef<Expr_Bool> theExpr = spFromClause(*iter, theTrue, theFalse);
		if (not theExpr || theExpr == theFalse)
			return theFalse;
		else if (result == theTrue)
			result = theExpr;
		else
			result &= theExpr;
		}
	return result;
	}

CNF sAsCNF(const ZRef<Expr_Bool>& iExpr)
	{ return Visitor_AsCNF().Do(iExpr); }

} // namespace Util_Expr_Bool
} // namespace ZooLib
