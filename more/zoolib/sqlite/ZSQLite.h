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

#ifndef __ZSQLite__
#define __ZSQLite__ 1
#include "zconfig.h"

#include "zoolib/ZRef_Counted.h"
#include "zoolib/ZUnicodeString.h"
#include "zoolib/ZVal_Any.h"

#include <sqlite3.h>

NAMESPACE_ZOOLIB_BEGIN
namespace ZSQLite {

// =================================================================================================
#pragma mark -
#pragma mark * ZSQLite

class DB : public ZCounted
	{
public:
	DB(const string8& iPath);
	virtual ~DB();

	sqlite3* GetDB();

private:
	sqlite3* fDB;
	};

// =================================================================================================
#pragma mark -
#pragma mark * Iter

class Iter : public ZCounted
	{
	Iter(ZRef<DB> iDB, const string8& iSQL, size_t iPosition);

public:
	Iter(ZRef<DB> iDB, const string8& iSQL);
	virtual ~Iter();

	ZRef<Iter> Clone(bool iRewound);
	void Rewind();
	bool HasValue();
	void Advance();

	size_t Count();
	string8 NameOf(size_t iIndex);
	ZAny Get(size_t iIndex);

private:
	void pAdvance();

	ZRef<DB> fDB;
	const string8 fSQL;
	sqlite3_stmt* fStmt;
	bool fHasValue;
	uint64 fPosition;
	};

} // namespace ZSQLite
NAMESPACE_ZOOLIB_END

#endif // __ZSQLite__
