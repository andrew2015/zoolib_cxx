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

#include "zoolib/valbase/ZValBase.h"
#include "zoolib/zqe/ZQE_Iterator_Any.h"
#include "zoolib/zqe/ZQE_Result_Any.h"
#include "zoolib/zqe/ZQE_Visitor_Expr_Rel_DoMakeIterator.h"
#include "zoolib/zra/ZRA_Expr_Rel_Concrete.h"
#include "zoolib/zra/ZRA_Expr_Rel_Restrict.h"

NAMESPACE_ZOOLIB_BEGIN
namespace ZValBase {

// =================================================================================================
#pragma mark -
#pragma mark * Visitor_DoMakeIterator

namespace ZANONYMOUS {

class Visitor_DoMakeIterator
:	public virtual ZQE::Visitor_Expr_Rel_DoMakeIterator
,	public virtual ZRA::Visitor_Expr_Rel_Concrete
,	public virtual ZRA::Visitor_Expr_Rel_Restrict
	{
public:
	virtual void Visit_Expr_Rel_Concrete(ZRef<ZRA::Expr_Rel_Concrete> iExpr);	
	virtual void Visit_Expr_Rel_Project(ZRef<ZRA::Expr_Rel_Project> iExpr);
	virtual void Visit_Expr_Rel_Rename(ZRef<ZRA::Expr_Rel_Rename> iExpr);
	virtual void Visit_Expr_Rel_Restrict(ZRef<ZRA::Expr_Rel_Restrict> iExpr);
	virtual void Visit_Expr_Rel_Select(ZRef<ZRA::Expr_Rel_Select> iExpr);
	};

void Visitor_DoMakeIterator::Visit_Expr_Rel_Concrete(ZRef<ZRA::Expr_Rel_Concrete> iExpr)
	{
	if (ZRef<Expr_Rel_Concrete> theExpr = iExpr.DynamicCast<Expr_Rel_Concrete>())
		this->pSetResult(theExpr->MakeIterator());
	}

void Visitor_DoMakeIterator::Visit_Expr_Rel_Project(ZRef<ZRA::Expr_Rel_Project> iExpr)
	{
	if (ZRef<ZQE::Iterator> theIterator = this->Do(iExpr->GetOp0()))
		this->pSetResult(new ZQE::Iterator_Any_Project(theIterator, iExpr->GetRelHead()));
	}

void Visitor_DoMakeIterator::Visit_Expr_Rel_Rename(ZRef<ZRA::Expr_Rel_Rename> iExpr)
	{
	if (ZRef<ZQE::Iterator> theIterator = this->Do(iExpr->GetOp0()))
		{
		this->pSetResult(
			new ZQE::Iterator_Any_Rename(theIterator, iExpr->GetNew(), iExpr->GetOld()));
		}
	}

void Visitor_DoMakeIterator::Visit_Expr_Rel_Restrict(ZRef<ZRA::Expr_Rel_Restrict> iExpr)
	{
	if (ZRef<ZQE::Iterator> theIterator = this->Do(iExpr->GetOp0()))
		this->pSetResult(new ZQE::Iterator_Any_Restrict(theIterator, iExpr->GetValCondition()));
	}

void Visitor_DoMakeIterator::Visit_Expr_Rel_Select(ZRef<ZRA::Expr_Rel_Select> iExpr)
	{
	if (ZRef<ZQE::Iterator> theIterator = this->Do(iExpr->GetOp0()))
		this->pSetResult(new ZQE::Iterator_Any_Select(theIterator, iExpr->GetExpr_Logic()));
	}

} // anonymous namespace

// =================================================================================================
#pragma mark -
#pragma mark * Expr_Rel_Concrete_Dummy

class Expr_Rel_Concrete_Dummy : public ZRA::Expr_Rel_Concrete
	{
public:
	Expr_Rel_Concrete_Dummy(const ZRA::RelHead& iRelHead, const string8& iName);

// From ZRA::Expr_Rel_Concrete
	virtual ZRA::RelHead GetRelHead();

	virtual string8 GetName();

private:
	const ZRA::RelHead fRelHead;
	const string8 fName;
	};

Expr_Rel_Concrete_Dummy::Expr_Rel_Concrete_Dummy(const ZRA::RelHead& iRelHead, const string8& iName)
:	fRelHead(iRelHead)
,	fName(iName)
	{}

ZRA::RelHead Expr_Rel_Concrete_Dummy::GetRelHead()
	{ return fRelHead; }

string8 Expr_Rel_Concrete_Dummy::GetName()
	{ return fName; }

// =================================================================================================
#pragma mark -
#pragma mark * ZValBase pseudo constructors

// Put these outside of ZValBase? Perhaps over in ZQL?

ZRef<ZRA::Expr_Rel> sConcrete()
	{ return sConcrete(ZRA::RelHead::sUniversal()); }

ZRef<ZRA::Expr_Rel> sConcrete(const ZRA::RelHead& iRelHead)
	{ return new Expr_Rel_Concrete_Dummy(iRelHead, string8()); }

ZRef<ZRA::Expr_Rel> sConcrete(const ZRA::RelHead& iRelHead, const string8& iName)
	{ return new Expr_Rel_Concrete_Dummy(iRelHead, iName); }

ZRef<ZQE::Iterator> sIterator(ZRef<ZRA::Expr_Rel> iExpr)
	{ return Visitor_DoMakeIterator().Do(iExpr); }

} // namespace ZValBase
NAMESPACE_ZOOLIB_END
