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

#ifndef __ZRA_Expr_Rel_Concrete__
#define __ZRA_Expr_Rel_Concrete__ 1
#include "zconfig.h"

#include "zoolib/ZExpr_Op_T.h"
#include "zoolib/ZUnicodeString.h"
#include "zoolib/zra/ZRA_Expr_Rel.h"
#include "zoolib/zra/ZRA_RelHead.h"

namespace ZooLib {
namespace ZRA {

class Visitor_Expr_Rel_Concrete;

// =================================================================================================
#pragma mark -
#pragma mark * ConcreteDomain

class ConcreteDomain : public ZCounted
	{
public:
	ConcreteDomain();
	virtual ~ConcreteDomain();
	};

// =================================================================================================
#pragma mark -
#pragma mark * Expr_Rel_Concrete

class Expr_Rel_Concrete
:	public virtual Expr_Rel
,	public virtual ZExpr_Op0_T<Expr_Rel>
	{
	typedef ZExpr_Op0_T<Expr_Rel> inherited;
public:
// From ZExpr_Op0_T<Expr_Rel>
	virtual void Accept_Expr_Op0(ZVisitor_Expr_Op0_T<Expr_Rel>& iVisitor);

	virtual ZRef<Expr_Rel> Self();
	virtual ZRef<Expr_Rel> Clone();

// Our protocol
	virtual void Accept_Expr_Rel_Concrete(Visitor_Expr_Rel_Concrete& iVisitor);

	virtual RelHead GetConcreteRelHead() = 0;
	virtual ZRef<ConcreteDomain> GetConcreteDomain();
	virtual string8 GetName();

// Experimental. Perhaps should be something we ask of the ConcreteDomain?
	virtual string8 GetDescription();
	};

// =================================================================================================
#pragma mark -
#pragma mark * Visitor_Expr_Rel_Concrete

class Visitor_Expr_Rel_Concrete
:	public virtual ZVisitor_Expr_Op0_T<Expr_Rel>
	{
	typedef ZVisitor_Expr_Op0_T<Expr_Rel> inherited;
public:
	virtual void Visit_Expr_Rel_Concrete(ZRef<Expr_Rel_Concrete> iExpr);
	};

// =================================================================================================
#pragma mark -
#pragma mark * Expr_Rel_Concrete_Simple

class Expr_Rel_Concrete_Simple : public Expr_Rel_Concrete
	{
public:
	Expr_Rel_Concrete_Simple(
		ZRef<ConcreteDomain> iConcreteDomain, const string8& iName, const RelHead& iRelHead);

// From Expr_Rel_Concrete
	virtual RelHead GetConcreteRelHead();
	virtual ZRef<ConcreteDomain> GetConcreteDomain();
	virtual string8 GetName();

private:
	ZRef<ConcreteDomain> fConcreteDomain;
	const string8 fName;
	const RelHead fRelHead;
	};

ZRef<Expr_Rel> sConcrete(
	ZRef<ConcreteDomain> iConcreteDomain, const string8& iName, const RelHead& iRelHead);

ZRef<Expr_Rel> sConcrete(const RelHead& iRelHead);

} // namespace ZRA
} // namespace ZooLib

#endif // __ZRA_Expr_Rel_Concrete__
