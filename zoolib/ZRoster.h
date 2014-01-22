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

#ifndef __ZRoster_h__
#define __ZRoster_h__ 1
#include "zconfig.h"

#include "zoolib/ZCallable.h"
#include "zoolib/ZCompat_NonCopyable.h"

#include <set>

namespace ZooLib {

// =================================================================================================
// MARK: - ZRoster

class ZRoster
:	public ZCounted
,	NonCopyable
	{
public:
	class Entry;

	ZRoster();

	ZRoster(const ZRef<ZCallable_Void>& iCallable_Change,
		const ZRef<ZCallable_Void>& iCallable_Gone);

	virtual ~ZRoster();

// From ZCounted
	virtual void Finalize();

// Our protocol
	ZRef<Entry> MakeEntry();

	ZRef<Entry> MakeEntry(const ZRef<ZCallable_Void>& iCallable_Broadcast,
		const ZRef<ZCallable_Void>& iCallable_Gone);
	
	void Broadcast();

	size_t Count();
	void Wait(size_t iCount);
	bool WaitFor(double iTimeout, size_t iCount);
	bool WaitUntil(ZTime iDeadline, size_t iCount);

private:
	void pFinalizeEntry(Entry* iEntry, const ZRef<ZCallable_Void>& iCallable_Gone);

	ZMtx fMtx;
	ZCnd fCnd;
	std::set<Entry*> fEntries;
	const ZRef<ZCallable_Void> fCallable_Change;
	const ZRef<ZCallable_Void> fCallable_Gone;

	friend class Entry;
	};

// =================================================================================================
// MARK: - ZRoster::Entry

class ZRoster::Entry
:	public ZCounted
,	NonCopyable
	{
private:
	Entry(
		const ZRef<ZRoster>& iRoster,
		const ZRef<ZCallable_Void>& iCallable_Broadcast,
		const ZRef<ZCallable_Void>& iCallable_Gone);

public:
	virtual ~Entry();

// From ZCounted
	virtual void Finalize();

private:
	ZWeakRef<ZRoster> fRoster;

	const ZRef<ZCallable_Void> fCallable_Broadcast;
	const ZRef<ZCallable_Void> fCallable_Gone;

	friend class ZRoster;
	};

} // namespace ZooLib

#endif // __ZYad_h__
