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

#include "zoolib/RelationalAlgebra/Expr_Rel_Intersect.h"

#include "zoolib/Compare_Ref.h"

namespace ZooLib {

// =================================================================================================
#pragma mark - sCompare_T

template <>
int sCompare_T(const RelationalAlgebra::Expr_Rel_Intersect& iL,
	const RelationalAlgebra::Expr_Rel_Intersect& iR)
	{
	if (int compare = sCompare_T(iL.GetOp0(), iR.GetOp0()))
		return compare;

	return sCompare_T(iL.GetOp1(), iR.GetOp1());
	}

ZMACRO_CompareRegistration_T(RelationalAlgebra::Expr_Rel_Intersect)

namespace RelationalAlgebra {

// =================================================================================================
#pragma mark - Expr_Rel_Intersect

Expr_Rel_Intersect::Expr_Rel_Intersect(const ZP<Expr_Rel>& iOp0, const ZP<Expr_Rel>& iOp1)
:	inherited(iOp0, iOp1)
	{}

void Expr_Rel_Intersect::Accept_Expr_Op2(Visitor_Expr_Op2_T<Expr_Rel>& iVisitor)
	{
	if (Visitor_Expr_Rel_Intersect* theVisitor = sDynNonConst<Visitor_Expr_Rel_Intersect>(&iVisitor))
		this->Accept_Expr_Rel_Intersect(*theVisitor);
	else
		inherited::Accept_Expr_Op2(iVisitor);
	}

ZP<Expr_Rel> Expr_Rel_Intersect::Self()
	{ return this; }

ZP<Expr_Rel> Expr_Rel_Intersect::Clone(const ZP<Expr_Rel>& iOp0, const ZP<Expr_Rel>& iOp1)
	{ return new Expr_Rel_Intersect(iOp0, iOp1); }

void Expr_Rel_Intersect::Accept_Expr_Rel_Intersect(Visitor_Expr_Rel_Intersect& iVisitor)
	{ iVisitor.Visit_Expr_Rel_Intersect(this); }

// =================================================================================================
#pragma mark - Visitor_Expr_Rel_Intersect

void Visitor_Expr_Rel_Intersect::Visit_Expr_Rel_Intersect(const ZP<Expr_Rel_Intersect>& iExpr)
	{ this->Visit_Expr_Op2(iExpr); }

// =================================================================================================
#pragma mark - Relational operators

ZP<Expr_Rel_Intersect> sIntersect(
	const ZP<Expr_Rel>& iLHS, const ZP<Expr_Rel>& iRHS)
	{
	if (iLHS && iRHS)
		return new Expr_Rel_Intersect(iLHS, iRHS);
	sSemanticError("sIntersect, LHS and/or RHS are null");
	return null;
	}

ZP<Expr_Rel> operator&(
	const ZP<Expr_Rel>& iLHS, const ZP<Expr_Rel>& iRHS)
	{ return sIntersect(iLHS, iRHS); }

} // namespace RelationalAlgebra
} // namespace ZooLib
