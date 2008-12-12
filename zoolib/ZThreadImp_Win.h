/* -------------------------------------------------------------------------------------------------
Copyright (c) 2008 Andrew Green
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

#ifndef __ZThreadImp_Win__
#define __ZThreadImp_Win__ 1
#include "zconfig.h"

#include "zoolib/ZCONFIG_API.h"
#include "zoolib/ZCONFIG_SPI.h"

#ifndef ZCONFIG_API_Avail__ThreadImp_Win
#	define ZCONFIG_API_Avail__ThreadImp_Win ZCONFIG_SPI_Enabled(Win)
#endif

#ifndef ZCONFIG_API_Desired__ThreadImp_Win
#	define ZCONFIG_API_Desired__ThreadImp_Win 1
#endif

#if ZCONFIG_API_Enabled(ThreadImp_Win)

#include "zoolib/ZCompat_NonCopyable.h"
#include "zoolib/ZThreadImp_T.h"

// Unfortunately we need to pull in the Windows headers so
// the definition of CRITICAL_SECTION is visible.
#include "zoolib/ZWinHeader.h"

namespace ZooLib {

class ZCnd_Win;

// =================================================================================================
#pragma mark -
#pragma mark * ZTSS_Win

class ZTSS_Win : NonCopyable
	{
	ZTSS_Win();
public:
	typedef DWORD Key;
	typedef LPVOID Value;

	static Key sCreate();
	static void sFree(Key iKey);

	static void sSet(Key iKey, Value iValue);
	static Value sGet(Key iKey);
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZSemTimeout_Win

class ZSemTimeout_Win : NonCopyable
	{
public:
	ZSemTimeout_Win();

	~ZSemTimeout_Win();

	void Wait();
	bool Wait(double iTimeout);
	void Signal();

protected:
	HANDLE fHANDLE;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZMtx_Win

class ZMtx_Win : NonCopyable
	{
public:
	ZMtx_Win();
	~ZMtx_Win();

	void Acquire();
	void Release();

protected:
	CRITICAL_SECTION fCRITICAL_SECTION;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZCnd_Win

class ZCnd_Win : ZCnd_T<ZMtx_Win, ZSemTimeout_Win>
	{
public:
	ZCnd_Win();

	~ZCnd_Win();

	void Wait(ZMtx_Win& iMtx);
	void Wait(ZMtx_Win& iMtx, double iTimeout);
	void Signal();
	void Broadcast();
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZThreadImp_Win

class ZThreadImp_Win : NonCopyable
	{
private:
	ZThreadImp_Win();

public:
	typedef unsigned (__stdcall* Proc_t)(void *);
	typedef unsigned int ID;

	static ID sCreate(size_t iStackSize, Proc_t iProc, void* iParam);
	static ID sID();
	};

} // namespace ZooLib

#endif // ZCONFIG_API_Enabled(ThreadImp_Win)

#endif // __ZThreadImp_Win__
