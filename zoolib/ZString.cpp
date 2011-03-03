/* -------------------------------------------------------------------------------------------------
Copyright (c) 2000 Andrew Green and Learning in Motion, Inc.
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

#include "zoolib/ZString.h"

#include "zoolib/ZCompat_algorithm.h" // For min
#include "zoolib/ZCompat_string.h" // For strncasecmp
#include "zoolib/ZDebug.h"
#include "zoolib/ZMemory.h" // For ZMemCopy
#include "zoolib/ZUnicode.h"

#include <ctype.h>
#include <stdexcept>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

using std::min;
using std::runtime_error;
using std::string;

#if ZCONFIG(Compiler, MSVC)
#	define vsnprintf _vsnprintf
#	define strncasecmp _strnicmp
#endif

#ifndef va_copy
#	define va_copy(dest, src) dest = src
#endif

namespace ZooLib {

// =================================================================================================

string ZStringf(const char* iString, ...)
	{
	va_list args;
	va_start(args, iString);
	string result = ZStringf(iString, args);
	va_end(args);

	return result;
	}

string ZStringf(const char* iString, std::va_list iArgs)
	{
	string result(512, ' ');
	while (true)
		{
		va_list localArgs;
		va_copy(localArgs, iArgs);

		int count = vsnprintf(const_cast<char*>(result.data()), result.size(), iString, localArgs);

		va_end(localArgs);

		if (count < 0)
			{
			// Handle -ve result from glibc prior to version 2.1 by growing the string.
			result.resize(result.size() * 2);
			}
		else if (size_t(count) > result.size())
			{
			// Handle C99 standard, where count indicates how much space would have been needed.
			result.resize(count);
			}
		else
			{
			// The string fitted
			result.resize(count);
			break;
			}
		}

	return result;
	}

// =================================================================================================

string ZString::sFormat(const char* iString, ...)
	{
	va_list args;
	va_start(args, iString);
	string result = sFormat(iString, args);
	va_end(args);

	return result;
	}

string ZString::sFormat(const char* iString, va_list iArgs)
	{ return ZStringf(iString, iArgs); }

// =================================================================================================

string ZString::sSubstitute(const string& iString,
	const string& iS0)
	{
	return sSubstitute(iString, &iS0, 1);
	}

string ZString::sSubstitute(const string& iString,
	const string& iS0,
	const string& iS1)
	{
	string theParams[2];
	theParams[0] = iS0;
	theParams[1] = iS1;
	return sSubstitute(iString, theParams, 2);
	}

string ZString::sSubstitute(const string& iString,
	const string& iS0,
	const string& iS1,
	const string& iS2)
	{
	string theParams[3];
	theParams[0] = iS0;
	theParams[1] = iS1;
	theParams[2] = iS2;
	return sSubstitute(iString, theParams, 3);
	}

string ZString::sSubstitute(const string& iString,
	const string& iS0,
	const string& iS1,
	const string& iS2,
	const string& iS3)
	{
	string theParams[4];
	theParams[0] = iS0;
	theParams[1] = iS1;
	theParams[2] = iS2;
	theParams[3] = iS3;
	return sSubstitute(iString, theParams, 4);
	}

string ZString::sSubstitute(const string& iString,
	const string& iS0,
	const string& iS1,
	const string& iS2,
	const string& iS3,
	const string& iS4)
	{
	string theParams[5];
	theParams[0] = iS0;
	theParams[1] = iS1;
	theParams[2] = iS2;
	theParams[3] = iS3;
	theParams[4] = iS4;
	return sSubstitute(iString, theParams, 5);
	}

/// Return iString with occurrences of '%' followed by [0-9] replaced by strings from iParams.
/** A '%' followed by any character other than [0-9] will cause the following character to
appear in the result, so '%%' will appear as '%' in the result. If a substitution is indicated but
iParams/iParamsCount does not have the required string then an empty string is used.
*/
string ZString::sSubstitute(const string& iString, const string* iParams, size_t iParamsCount)
	{
	string result;
	string::size_type startPos = 0;
	while (true)
		{
		string::size_type nextPos = iString.find('%', startPos);
		if (nextPos == string::npos)
			{
			if (startPos == 0)
				{
				// We found no %'s at all, so we can simply return iString unchanged.
				return iString;
				}
			result += iString.substr(startPos, string::npos);
			break;
			}

		result += iString.substr(startPos, nextPos - startPos);

		// In the following comparison we add to pos rather than subtracting from
		// iString.size() because of the possibility of unsigned wraparound.
		if (nextPos + 2 > iString.size())
			break;

		char theFollowingChar = iString[nextPos + 1];
		if (theFollowingChar >= '0' && theFollowingChar <= '9')
			{
			if (size_t(theFollowingChar - '0') < iParamsCount)
				result += iParams[theFollowingChar - '0'];
			}
		else
			{
			result += theFollowingChar;
			}
		startPos = nextPos + 2;
		}
	return result;
	}

int ZString::sComparei(const string& iLeft, const string& iRight)
	{
	if (const size_t sizeLeft = iLeft.size())
		{
		if (const size_t sizeRight = iRight.size())
			{
			if (const int result =
				strncasecmp(iLeft.data(), iRight.data(), min(sizeLeft, sizeRight)))
				{
				return result;
				}
			else if (sizeLeft < sizeRight)
				return -1;
			else if (sizeRight < sizeLeft)
				return 1;
			else
				return 0;
			}
		else
			{
			return 1;
			}
		}
	else if (iRight.size())
		{
		return -1;
		}
	else
		{
		return 0;
		}
	}

bool ZString::sEquali(const string& iLeft, const string& iRight)
	{
	if (const size_t theSize = iLeft.size())
		{
		if (theSize != iRight.size())
			return false;

		return 0 == strncasecmp(iLeft.data(), iRight.data(), theSize);
		}
	else
		{
		return ! iRight.size();
		}
	}

bool ZString::sContainsi(const string& iTarget, const string& iCandidate)
	{
	return string::npos != ZUnicode::sToLower(iTarget).find(ZUnicode::sToLower(iCandidate));
	}

ZQ<int64> ZString::sQInt64(const string& iString)
	{
	if (iString.size())
		{
		int64 result;
		if (sscanf(iString.c_str(), "%lld", &result) > 0)
			return result;
		}
	return null;
	}

bool ZString::sQInt64(const string& iString, int64& oVal)
	{
	if (ZQ<int64> qInt64 = sQInt64(iString))
		{
		oVal = qInt64.Get();
		return true;
		}
	return false;
	}

int64 ZString::sDInt64(int64 iDefault, const string& iString)
	{
	if (ZQ<int64> qInt64 = sQInt64(iString))
		return qInt64.Get();
	return iDefault;
	}

int64 ZString::sInt64(const string& iString)
	{ return sDInt64(0, iString); }

ZQ<uint64> ZString::sQUInt64(const string& iString)
	{
	if (iString.size())
		{
		uint64 result;
		if (sscanf(iString.c_str(), "%llu", &result) > 0)
			return result;
		}
	return null;
	}

bool ZString::sQUInt64(const string& iString, uint64& oVal)
	{
	if (ZQ<uint64> qUInt64 = sQUInt64(iString))
		{
		oVal = qUInt64.Get();
		return true;
		}
	return false;
	}

uint64 ZString::sDUInt64(uint64 iDefault, const string& iString)
	{
	if (ZQ<uint64> qUInt64 = sQUInt64(iString))
		return qUInt64.Get();
	return iDefault;
	}

uint64 ZString::sUInt64(const string& iString)
	{ return sDUInt64(0, iString); }

ZQ<double> ZString::sQDouble(const string& iString)
	{
	if (iString.size())
		{
		double result;
		if (sscanf(iString.c_str(), "%lf", &result) > 0)
			return result;
		}
	return null;
	}

bool ZString::sQDouble(const string& iString, double& oVal)
	{
	if (ZQ<double> qDouble = sQDouble(iString))
		{
		oVal = qDouble.Get();
		return true;
		}
	return false;
	}

double ZString::sDDouble(double iDefault, const string& iString)
	{
	if (ZQ<double> qDouble = sQDouble(iString))
		return qDouble.Get();
	return iDefault;
	}

double ZString::sDouble(const string& iString)
	{ return sDDouble(0.0, iString); }

string ZString::sFresh(const string& iOther)
	{
	if (iOther.empty())
		return string();
	return string(iOther.data(), iOther.size());
	}

void ZString::sMakeFresh(string& ioString)
	{
	if (ioString.empty())
		ioString = string();
	else
		ioString = string(ioString.data(), ioString.size());
	}

// =================================================================================================

string ZString::sFromInt(int iInt)
	{
	char temp[21]; // 21 chars will work even for -ve 64 bit ints
	sprintf(temp, "%d", iInt);
	return temp;
	}

string ZString::sFromUInt64(uint64 iUInt64)
	{
	char temp[21];
	sprintf(temp, "%llu", iUInt64);
	return temp;
	}

string ZString::sHexFromInt(int iInt)
	{
	char temp[16];
	sprintf(temp, "%X", iInt);
	return temp;
	}

string ZString::sHexFromUInt64(uint64 iUInt64)
	{
	char temp[21];
	sprintf(temp, "%llX", iUInt64);
	return temp;
	}

string ZString::sFromPString(const unsigned char* inPString)
	{ return string((const char*)(&(inPString[1])), (long)inPString[0]); }

void ZString::sToPString(const string& inString, unsigned char* outPString, size_t inMaxLength)
	{
	size_t sourceLength = min(inString.size(), inMaxLength);
	if (sourceLength)
		ZMemCopy(&(outPString[1]), inString.data(), sourceLength);
	outPString[0] = sourceLength;
	}

void ZString::sToPString(const char* inString, unsigned char* outPString, size_t inMaxLength)
	{
	size_t sourceLength = min(strlen(inString), inMaxLength);
	if (sourceLength)
		ZMemCopy(&(outPString[1]), inString, sourceLength);
	outPString[0] = sourceLength;
	}

} // namespace ZooLib
