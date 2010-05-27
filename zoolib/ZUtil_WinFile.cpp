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

#include "zoolib/ZUtil_WinFile.h"

#if ZCONFIG_SPI_Enabled(Win)

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark * ZUtil_WinFile

namespace ZUtil_WinFile {

DWORD sFlush(HANDLE iHANDLE)
	{
	if (!::FlushFileBuffers(iHANDLE))
		return ::GetLastError();
	return 0;
	}

DWORD sRead(HANDLE iHANDLE, void* oDest, size_t iCount, size_t* oCountRead)
	{
	if (oCountRead)
		*oCountRead = 0;

	if (iCount)
		{
		DWORD countRead = 0;
		if (!::ReadFile(iHANDLE, oDest, iCount, &countRead, nullptr))
			return ::GetLastError();

		if (oCountRead)
			*oCountRead = countRead;
		}

	return 0;
	}

DWORD sWrite(HANDLE iHANDLE, const void* iSource, size_t iCount, size_t* oCountWritten)
	{
	if (oCountWritten)
		*oCountWritten = 0;

	if (iCount)
		{
		DWORD countWritten = 0;
		if (!::WriteFile(iHANDLE, iSource, iCount, &countWritten, nullptr))
			return ::GetLastError();

		if (oCountWritten)
			*oCountWritten = countWritten;
		}

	return 0;
	}

DWORD sRead(HANDLE iHANDLE, const uint64* iOffset,
	void* oDest, size_t iCount, size_t* oCountRead)
	{
	if (oCountRead)
		*oCountRead = 0;

	OVERLAPPED theOVERLAPPED = {0};
	if (iOffset)
		{
		theOVERLAPPED.Offset = *iOffset;
		theOVERLAPPED.OffsetHigh = *iOffset >> 32;
		}
	DWORD countRead = 0;
	if (!::ReadFile(iHANDLE, oDest, iCount, &countRead, &theOVERLAPPED))
		{
		int err = ::GetLastError();
		if (err != ERROR_IO_PENDING)
			return err;

		if (!::GetOverlappedResult(iHANDLE, &theOVERLAPPED, &countRead, TRUE))
			return ::GetLastError();
		}

	if (oCountRead)
		*oCountRead = countRead;

	return 0;
	}

DWORD sWrite(HANDLE iHANDLE, const uint64* iOffset,
	const void* iSource, size_t iCount, size_t* oCountWritten)
	{
	if (oCountWritten)
		*oCountWritten = 0;

	OVERLAPPED theOVERLAPPED;
	theOVERLAPPED.Internal = 0;
	theOVERLAPPED.InternalHigh = 0;
	if (iOffset)
		{
		theOVERLAPPED.Offset = *iOffset;
		theOVERLAPPED.OffsetHigh = *iOffset >> 32;
		}
	theOVERLAPPED.hEvent = 0;
	DWORD countWritten = 0;
	if (!::WriteFile(iHANDLE, iSource, iCount, &countWritten, &theOVERLAPPED))
		{
		int err = ::GetLastError();
		if (err != ERROR_IO_PENDING)
			return err;

		if (!::GetOverlappedResult(iHANDLE, &theOVERLAPPED, &countWritten, TRUE))
			return ::GetLastError();
		}

	if (oCountWritten)
		*oCountWritten = countWritten;

	return 0;
	}

} // namespace ZUtil_WinFile

} // namespace ZooLib

#endif // ZCONFIG_SPI_Enabled(Win)
