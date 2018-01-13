/* -------------------------------------------------------------------------------------------------
Copyright (c) 2003 Andrew Green and Learning in Motion, Inc.
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

#include "zoolib/ZTextCoder_iconv.h"

#if ZCONFIG_API_Enabled(TextCoder_iconv)

#include "zoolib/FunctionChain.h"

#include "zoolib/ZDebug.h"

#include <errno.h>
#include <stdexcept>

using std::runtime_error;
using std::string;
using std::vector;

namespace ZooLib {

static const size_t kBufSize = sStackBufferSize;

// =================================================================================================
#pragma mark -
#pragma mark Factory functions

namespace { // anonymous

class Make_Decoder
:	public FunctionChain<ZTextDecoder*, const string&>
	{
	virtual bool Invoke(Result_t& oResult, Param_t iParam)
		{
		oResult = new ZTextDecoder_iconv(iParam);
		return true;
		}
	} sMaker0;

class Make_Encoder
:	public FunctionChain<ZTextEncoder*, const string&>
	{
	virtual bool Invoke(Result_t& oResult, Param_t iParam)
		{
		oResult = new ZTextEncoder_iconv(iParam);
		return true;
		}
	} sMaker1;

} // anonymous namespace

// =================================================================================================
#pragma mark -
#pragma mark Helper functions

typedef size_t (*iconvproc_const_t)(iconv_t cd, const char**, size_t*, char**, size_t*);
static inline size_t spIconvIntermediary(iconvproc_const_t iProc,
	iconv_t cd, const char** inbuf, size_t *inbytesleft, char** outbuf, size_t *outbytesleft)
	{
	return iProc(cd, inbuf, inbytesleft, outbuf, outbytesleft);
	}

typedef size_t (*iconvproc_t)(iconv_t cd, char**, size_t*, char**, size_t*);
static inline size_t spIconvIntermediary(iconvproc_t iProc,
	iconv_t cd, const char** inbuf, size_t *inbytesleft, char** outbuf, size_t *outbytesleft)
	{
	char** inbufNonConst = const_cast<char**>(const_cast<char* const*>(inbuf));
	return iProc(cd, inbufNonConst, inbytesleft, outbuf, outbytesleft);
	}

size_t static spIconv(
	iconv_t cd, const char** inbuf, size_t *inbytesleft, char** outbuf, size_t *outbytesleft)
	{
	// Different versions of iconv.h may declare inbuf as being const or non-const.
	// Previously we detected a more modern (const-using) version of the iconv library by
	// checking for the preprocessor symbol _LIBICONV_VERSION. However, Chris Teplovs
	// discovered that even modern versions may declare a non-const inbuf, if iconv is
	// installed over the top of an older version.

	// So we don't know if inbuf should be 'const char**' or 'char**', and there's no
	// type to which we can unconditionally cast that will then be implicitly cast to
	// the appropriate type. So we pass iconv to the overloaded spIconvIntermediary function,
	// the appropriate version of which will be selected by the compiler. One version
	// simply calls the passed in function, the other does an appropriate cast.
	// The actual code generated will generally be identical to a simple invocation of iconv.

	return spIconvIntermediary(iconv, cd, inbuf, inbytesleft, outbuf, outbytesleft);
	}

// =================================================================================================
#pragma mark -
#pragma mark ZTextDecoder_iconv

static iconv_t spIconvOpenDecoder(const string& iSourceName)
	{
	iconv_t result;
	if (ZCONFIG(Endian, Big))
		result = ::iconv_open("UTF-32BE", iSourceName.c_str());
	else
		result = ::iconv_open("UTF-32LE", iSourceName.c_str());

	if (result == iconv_t(-1))
		return 0;
	return result;
	}

static iconv_t spOpenDecoder(const string& iSourceName)
	{
	if (iconv_t result = spIconvOpenDecoder(iSourceName))
		return result;

	// Work our way through any aliases ZTextCoder may know about
	vector<string> aliases;
	ZTextCoder::sGetAliases(iSourceName, aliases);
	for (vector<string>::iterator ii = aliases.begin(); ii != aliases.end(); ++ii)
		{
		if (iconv_t result = spIconvOpenDecoder(*ii))
			return result;
		}
	return 0;
	}

ZTextDecoder_iconv::ZTextDecoder_iconv(const string& iSourceName)
	{
	fConverter = spOpenDecoder(iSourceName);
	if (not fConverter)
		throw runtime_error("Couldn't open converter");
	}

ZTextDecoder_iconv::ZTextDecoder_iconv(const char* iSourceName)
	{
	fConverter = spOpenDecoder(iSourceName);
	if (not fConverter)
		throw runtime_error("Couldn't open converter");
	}

ZTextDecoder_iconv::~ZTextDecoder_iconv()
	{
	::iconv_close(fConverter);
	}

bool ZTextDecoder_iconv::Decode(
	const void* iSource, size_t iSourceBytes, size_t* oSourceBytes, size_t* oSourceBytesSkipped,
	UTF32* oDest, size_t iDestCU, size_t* oDestCU)
	{
	const char* localSource = static_cast<const char*>(iSource);
	size_t localSourceBytes = iSourceBytes;

	char* localDest = reinterpret_cast<char*>(oDest);
	size_t localDestBytes = iDestCU * sizeof(UTF32);

	size_t sourceBytesSkipped = 0;

	bool sourceComplete = true;
	while (localSourceBytes && localDestBytes)
		{
		if (size_t(-1) == spIconv(
			fConverter, &localSource, &localSourceBytes, &localDest, &localDestBytes))
			{
			int err = errno;
			if (err == EINVAL)
				{
				sourceComplete = false;
				break;
				}
			else if (err == E2BIG)
				{
				break;
				}
			else if (err == EILSEQ)
				{
				// Illegal source sequence. Advance by a byte and try again.
				++localSource;
				--localSourceBytes;
				++sourceBytesSkipped;
				}
			else
				{
				ZDebugStopf(1, "Unexpected error %d", err);
				}
			}
		}
	if (oSourceBytes)
		*oSourceBytes = localSource - static_cast<char*>(const_cast<void*>(iSource));
	if (oSourceBytesSkipped)
		*oSourceBytesSkipped = sourceBytesSkipped;
	if (oDestCU)
		*oDestCU = reinterpret_cast<UTF32*>(localDest) - oDest;
	return sourceComplete;
	}

void ZTextDecoder_iconv::Reset()
	{
	::iconv(fConverter, nullptr, 0, nullptr, 0);
	}

// =================================================================================================
#pragma mark -
#pragma mark ZTextEncoder_iconv

static iconv_t spIconvOpenEncoder(const string& iDestName)
	{
	iconv_t result;
	if (ZCONFIG(Endian, Big))
		result = ::iconv_open(iDestName.c_str(), "UTF-32BE");
	else
		result = ::iconv_open(iDestName.c_str(), "UTF-32LE");

	if (result == iconv_t(-1))
		return 0;
	return result;
	}

static iconv_t spOpenEncoder(const string& iDestName)
	{
	if (iconv_t result = spIconvOpenEncoder(iDestName))
		return result;

	// Work our way through any aliases ZTextCoder may know about
	vector<string> aliases;
	ZTextCoder::sGetAliases(iDestName, aliases);
	for (vector<string>::iterator ii = aliases.begin(); ii != aliases.end(); ++ii)
		{
		if (iconv_t result = spIconvOpenEncoder(*ii))
			return result;
		}
	return 0;
	}

ZTextEncoder_iconv::ZTextEncoder_iconv(const string& iDestName)
	{
	fConverter = spOpenEncoder(iDestName);
	if (not fConverter)
		throw runtime_error("Couldn't open converter");
	}

ZTextEncoder_iconv::ZTextEncoder_iconv(const char* iDestName)
	{
	fConverter = spOpenEncoder(iDestName);
	if (not fConverter)
		throw runtime_error("Couldn't open converter");
	}

ZTextEncoder_iconv::~ZTextEncoder_iconv()
	{
	::iconv_close(fConverter);
	}

void ZTextEncoder_iconv::Encode(const UTF32* iSource, size_t iSourceCU, size_t* oSourceCU,
					void* oDest, size_t iDestBytes, size_t* oDestBytes)
	{
	const char* localSource = static_cast<const char*>(static_cast<const void*>(iSource));
	size_t localSourceBytes = iSourceCU * sizeof(UTF32);

	char* localDest = static_cast<char*>(oDest);
	size_t localDestBytes = iDestBytes;

	while (localSourceBytes && localDestBytes)
		{
		if (size_t(-1) == spIconv(
			fConverter, &localSource, &localSourceBytes, &localDest, &localDestBytes))
			{
			int err = errno;
			if (err == EINVAL)
				{
				// This shouldn't happen.
				ZDebugStopf(1, ("A truncated source UTF32 sequence shouldn't be possible"));
				break;
				}
			else if (err == E2BIG)
				{
				break;
				}
			else if (err == EILSEQ)
				{
				// Illegal source sequence. Advance by one UTF32 CU and try again?
				localSource += 4;
				ZAssertStop(1, localSourceBytes >= 4);
				localSourceBytes -= 4;
				}
			else
				{
				ZDebugStopf(1, "Unexpected error %d", err);
				}
			}
		}
	if (oSourceCU)
		*oSourceCU = reinterpret_cast<const UTF32*>(localSource) - iSource;
	if (oDestBytes)
		*oDestBytes = localDest - static_cast<char*>(oDest);
	}

void ZTextEncoder_iconv::Reset()
	{
	::iconv(fConverter, nullptr, 0, nullptr, 0);
	}

} // namespace ZooLib

#endif // ZCONFIG_API_Enabled(TextCoder_iconv)