/* -------------------------------------------------------------------------------------------------
Copyright (c) 2014 Andrew Green
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

#ifndef __ZooLib_ChanW_UTF_h__
#define __ZooLib_ChanW_UTF_h__ 1
#include "zconfig.h"

#include "zoolib/ChanW.h"
#include "zoolib/UnicodeString.h"

#include <cstdarg>

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark 

typedef ChanW<UTF32> ChanW_UTF;

// =================================================================================================
#pragma mark -
#pragma mark 

/** \name String buffers, limiting and reporting both CU and CP
*/	//@{
void sWrite(const UTF32* iSource,
	size_t iCountCU, size_t* oCountCU, size_t iCountCP, size_t* oCountCP,
	const ChanW_UTF& iChanW);

void sWrite(const UTF16* iSource,
	size_t iCountCU, size_t* oCountCU, size_t iCountCP, size_t* oCountCP,
	const ChanW_UTF& iChanW);

void sWrite(const UTF8* iSource,
	size_t iCountCU, size_t* oCountCU, size_t iCountCP, size_t* oCountCP,
	const ChanW_UTF& iChanW);
//@}

// =================================================================================================
#pragma mark -
#pragma mark 

/** \name Zero-terminated strings
*/	//@{
bool sQWrite(const UTF32* iString, const ChanW_UTF& iChanW);
bool sQWrite(const UTF16* iString, const ChanW_UTF& iChanW);
bool sQWrite(const UTF8* iString, const ChanW_UTF& iChanW);

void sWriteMust(const UTF32* iString, const ChanW_UTF& iChanW);
void sWriteMust(const UTF16* iString, const ChanW_UTF& iChanW);
void sWriteMust(const UTF8* iString, const ChanW_UTF& iChanW);
//@}

// =================================================================================================
#pragma mark -
#pragma mark 

/** \name Standard library strings
*/	//@{
bool sQWrite(const string32& iString, const ChanW_UTF& iChanW);
bool sQWrite(const string16& iString, const ChanW_UTF& iChanW);
bool sQWrite(const string8& iString, const ChanW_UTF& iChanW);

void sWriteMust(const string32& iString, const ChanW_UTF& iChanW);
void sWriteMust(const string16& iString, const ChanW_UTF& iChanW);
void sWriteMust(const string8& iString, const ChanW_UTF& iChanW);
//@}

// =================================================================================================
#pragma mark -
#pragma mark 

/** \name Formatted strings
*/	//@{
void sWritefMust(const ChanW_UTF& iChanW,
	const UTF8* iString, ...)
	ZMACRO_Attribute_Format_Printf(2,3);

void sWritef(const ChanW_UTF& iChanW, size_t* oCount_CUProduced, size_t* oCount_CUWritten,
	const UTF8* iString, ...)
	ZMACRO_Attribute_Format_Printf(4,5);

void sWritevMust(const ChanW_UTF& iChanW,
	const UTF8* iString, va_list iArgs);

void sWritev(const ChanW_UTF& iChanW, size_t* oCount_CUProduced, size_t* oCount_CUWritten,
	const UTF8* iString, va_list iArgs);
//@}

// =================================================================================================
#pragma mark -
#pragma mark ChanW_UTF_Native32

typedef ChanW_UTF ChanW_UTF_Native32;

// =================================================================================================
#pragma mark -
#pragma mark ChanW_UTF_Native16

class ChanW_UTF_Native16
:	public ChanW_UTF
	{
public:
// From ChanW_UTF (aka ChanW<UTF32>)
	virtual size_t QWrite(const UTF32* iSource, size_t iCountCU);

// Our protocol
	virtual size_t QWriteUTF16(const UTF16* iSource, size_t iCountCU) = 0;
	};

// =================================================================================================
#pragma mark -
#pragma mark ChanW_UTF_Native8

class ChanW_UTF_Native8
:	public ChanW_UTF
	{
public:
// From ChanW_UTF (aka ChanW<UTF32>)
	virtual size_t QWrite(const UTF32* iSource, size_t iCountCU);

// Our protocol
	virtual size_t QWriteUTF8(const UTF8* iSource, size_t iCountCU) = 0;
	};
} // namespace ZooLib

#endif // __ZooLib_ChanW_UTF_h__
