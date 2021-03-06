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

#include "zoolib/RelationalAlgebra/Expr_Rel_Embed.h"

#include "zoolib/Compare_Ref.h"
#include "zoolib/Compare_string.h"

namespace ZooLib {

// =================================================================================================
#pragma mark - sCompare_T

template <>
int sCompare_T(const RelationalAlgebra::Expr_Rel_Embed& iL,
	const RelationalAlgebra::Expr_Rel_Embed& iR)
	{
	if (int compare = sCompare_T(iL.GetBoundNames(), iR.GetBoundNames()))
		return compare;

	if (int compare = sCompare_T(iL.GetColName(), iR.GetColName()))
		return compare;

	if (int compare = sCompare_T(iL.GetOp0(), iR.GetOp0()))
		return compare;

	return sCompare_T(iL.GetOp1(), iR.GetOp1());
	}

ZMACRO_CompareRegistration_T(RelationalAlgebra::Expr_Rel_Embed)

namespace RelationalAlgebra {

// =================================================================================================
#pragma mark - Expr_Rel_Embed

Expr_Rel_Embed::Expr_Rel_Embed(const ZP<Expr_Rel>& iOp0, const RelHead& iBoundNames,
	const ColName& iColName, const ZP<Expr_Rel>& iEmbedee)
:	inherited(iOp0, iEmbedee)
,	fBoundNames(iBoundNames)
,	fColName(iColName)
	{}

void Expr_Rel_Embed::Accept(const Visitor& iVisitor)
	{
	if (Visitor_Expr_Rel_Embed* theVisitor = sDynNonConst<Visitor_Expr_Rel_Embed>(&iVisitor))
		this->Accept_Expr_Rel_Embed(*theVisitor);
	else
		inherited::Accept(iVisitor);
	}

void Expr_Rel_Embed::Accept_Expr_Op2(Visitor_Expr_Op2_T<Expr_Rel>& iVisitor)
	{
	if (Visitor_Expr_Rel_Embed* theVisitor = sDynNonConst<Visitor_Expr_Rel_Embed>(&iVisitor))
		this->Accept_Expr_Rel_Embed(*theVisitor);
	else
		inherited::Accept_Expr_Op2(iVisitor);
	}

ZP<Expr_Rel> Expr_Rel_Embed::Self()
	{ return this; }

ZP<Expr_Rel> Expr_Rel_Embed::Clone(const ZP<Expr_Rel>& iOp0, const ZP<Expr_Rel>& iOp1)
	{ return new Expr_Rel_Embed(iOp0, fBoundNames, fColName, iOp1); }

void Expr_Rel_Embed::Accept_Expr_Rel_Embed(Visitor_Expr_Rel_Embed& iVisitor)
	{ iVisitor.Visit_Expr_Rel_Embed(this); }

const RelHead& Expr_Rel_Embed::GetBoundNames() const
	{ return fBoundNames; }

const ColName& Expr_Rel_Embed::GetColName() const
	{ return fColName; }

// =================================================================================================
#pragma mark - Visitor_Expr_Rel_Embed

void Visitor_Expr_Rel_Embed::Visit_Expr_Rel_Embed(const ZP<Expr_Rel_Embed>& iExpr)
	{ this->Visit_Expr_Op2(iExpr); }

// =================================================================================================
#pragma mark - Relational operators

ZP<Expr_Rel> sEmbed(const ZP<Expr_Rel>& iOp0, const RelHead& iBoundNames,
	const ColName& iColName, const ZP<Expr_Rel>& iEmbedee)
	{ return new Expr_Rel_Embed(iOp0, iBoundNames, iColName, iEmbedee); }

} // namespace RelationalAlgebra
} // namespace ZooLib
