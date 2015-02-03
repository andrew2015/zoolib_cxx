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

#include "zoolib/ChanW_UTF_string.h"
#include "zoolib/Unicode.h"
#include "zoolib/Util_Chan.h"

#include "zoolib/ZUtil_Strim.h"

#include "zoolib/ZCompat_cmath.h" // For pow, NAN and INFINITY
#include "zoolib/ZDebug.h"
#include "zoolib/ZStrim_Escaped.h"
#include "zoolib/ZStrimR_Boundary.h"

#include <vector>

namespace ZooLib {
namespace ZUtil_Strim {

static void spThrowParseException(const std::string& iWhat)
	{ throw ParseException(iWhat); }

// =================================================================================================
#pragma mark -
#pragma mark ZUtil_Strim

static bool spIsWhitespace(UTF32 iCP)
	{
	if (Unicode::sIsWhitespace(iCP))
		return true;

	if (iCP == 0xFEFF)
		return true;

	return false;
	}

bool sTryRead_CP(const ZStrimU& iStrimU, UTF32 iCP)
	{
	// Ensure that we only try to read a valid CP, one that
	// can actually be returned by ReadCP.
	ZAssertStop(2, Unicode::sIsValid(iCP));

	UTF32 theCP;
	if (not sQRead(theCP, iStrimU))
		return false;

	if (theCP == iCP)
		return true;

	sUnread(theCP, iStrimU);
	return false;
	}

// -----------------

bool sTryRead_CaselessString(const ZStrimU& iStrimU, const string8& iTarget)
	{
	string8::const_iterator targetIter = iTarget.begin();
	string8::const_iterator targetEnd = iTarget.end();

	std::vector<UTF32> stack;
	for (;;)
		{
		UTF32 targetCP;
		if (not Unicode::sReadInc(targetIter, targetEnd, targetCP))
			{
			// Exhausted target, and thus successful.
			return true;
			}

		UTF32 candidateCP;
		if (not sQRead(candidateCP, iStrimU))
			{
			// Exhausted strim.
			break;
			}
		stack.push_back(candidateCP);

		if (Unicode::sToLower(targetCP) != Unicode::sToLower(candidateCP))
			{
			// Mismatched code points.
			break;
			}
		}

	while (!stack.empty())
		{
		iStrimU.Unread(stack.back());
		stack.pop_back();
		}
	return false;
	}

// -----------------

bool sTryRead_Digit(const ZStrimU& iStrimU, int& oDigit)
	{
	UTF32 theCP;
	if (not sQRead(theCP, iStrimU))
		return false;

	if (theCP >= '0' && theCP <= '9')
		{
		oDigit = theCP - '0';
		return true;
		}

	sUnread(theCP, iStrimU);
	return false;
	}

bool sTryRead_HexDigit(const ZStrimU& iStrimU, int& oDigit)
	{
	UTF32 theCP;
	if (not sQRead(theCP, iStrimU))
		return false;

	if (theCP >= '0' && theCP <= '9')
		{
		oDigit = theCP - '0';
		return true;
		}

	if (theCP >= 'a' && theCP <= 'f')
		{
		oDigit = theCP - 'a' + 10;
		return true;
		}

	if (theCP >= 'A' && theCP <= 'F')
		{
		oDigit = theCP - 'A' + 10;
		return true;
		}

	sUnread(theCP, iStrimU);
	return false;
	}

bool sTryRead_SignedGenericInteger(const ZStrimU& iStrimU, int64& oInt64)
	{
	bool isNegative = false;
	bool hadPrefix = sTryRead_Sign(iStrimU, isNegative);

	if (sTryRead_CP(iStrimU, '0'))
		{
		UTF32 theCP;
		if (not sQRead(theCP, iStrimU))
			{
			oInt64 = 0;
			return true;
			}

		if (theCP == 'X' || theCP == 'x')
			{
			if (sTryRead_HexInteger(iStrimU, oInt64))
				{
				if (isNegative)
					oInt64 = -oInt64;
				return true;
				}
			spThrowParseException("Expected a valid hex integer after '0x' prefix");
			}

		sUnread(theCP, iStrimU);
		if (not Unicode::sIsDigit(theCP))
			{
			oInt64 = 0;
			return true;
			}

		/*bool readDecimal = */sTryRead_DecimalInteger(iStrimU, oInt64);
		// We know that the first CP to be read is a digit, so sTryRead_DecimalInteger can't fail.
		if (isNegative)
			oInt64 = -oInt64;
		return true;
		}

	if (sTryRead_DecimalInteger(iStrimU, oInt64))
		{
		if (isNegative)
			oInt64 = -oInt64;
		return true;
		}

	if (hadPrefix)
		{
		// We've already absorbed a plus or minus sign, hence we have a parse exception.
		if (isNegative)
			spThrowParseException("Expected a valid integer after '-' prefix");
		else
			spThrowParseException("Expected a valid integer after '+' prefix");
		}

	return false;
	}

bool sTryRead_HexInteger(const ZStrimU& iStrimU, int64& oInt64)
	{
	oInt64 = 0;
	for (bool gotAny = false; /*no test*/; gotAny = true)
		{
		int curDigit;
		if (not sTryRead_HexDigit(iStrimU, curDigit))
			return gotAny;
		oInt64 *= 16;
		oInt64 += curDigit;
		}
	}

bool sTryRead_Sign(const ZStrimU& iStrimU, bool& oIsNegative)
	{
	if (sTryRead_CP(iStrimU, '-'))
		{
		oIsNegative = true;
		return true;
		}
	else if (sTryRead_CP(iStrimU, '+'))
		{
		oIsNegative = false;
		return true;
		}
	return false;
	}

bool sTryRead_Mantissa(const ZStrimU& iStrimU, int64& oInt64, double& oDouble, bool& oIsDouble)
	{
	using namespace ZUtil_Strim;

	oInt64 = 0;
	oDouble = 0;
	oIsDouble = false;

	for (bool gotAny = false; /*no test*/; gotAny = true)
		{
		int curDigit;
		if (not sTryRead_Digit(iStrimU, curDigit))
			return gotAny;

		if (not oIsDouble)
			{
			int64 priorInt64 = oInt64;
			oInt64 *= 10;
			oInt64 += curDigit;
			if (oInt64 < priorInt64)
				{
				// We've overflowed.
				oIsDouble = true;
				}
			}
		oDouble *= 10;
		oDouble += curDigit;
		}
	}

bool sTryRead_DecimalInteger(const ZStrimU& iStrimU, int64& oInt64)
	{
	oInt64 = 0;
	for (bool gotAny = false; /*no test*/; gotAny = true)
		{
		int curDigit;
		if (not sTryRead_Digit(iStrimU, curDigit))
			return gotAny;
		oInt64 *= 10;
		oInt64 += curDigit;
		}
	}

bool sTryRead_SignedDecimalInteger(const ZStrimU& iStrimU, int64& oInt64)
	{
	bool isNegative = false;
	bool hadSign = sTryRead_Sign(iStrimU, isNegative);
	if (sTryRead_DecimalInteger(iStrimU, oInt64))
		{
		if (isNegative)
			oInt64 = -oInt64;
		return true;
		}

	if (hadSign)
		spThrowParseException("Expected a valid integer after sign prefix");

	return false;
	}

bool sTryRead_DecimalNumber(const ZStrimU& iStrimU, int64& oInt64, double& oDouble, bool& oIsDouble)
	{
	using namespace ZUtil_Strim;

	if (sTryRead_CaselessString(iStrimU, "nan"))
		{
		oIsDouble = true;
		oDouble = NAN;
		return true;
		}

	if (sTryRead_CaselessString(iStrimU, "inf"))
		{
		oIsDouble = true;
		oDouble = INFINITY;
		return true;
		}

	if (not sTryRead_Mantissa(iStrimU, oInt64, oDouble, oIsDouble))
		return false;

	if (sTryRead_CP(iStrimU, '.'))
		{
		oIsDouble = true;
		double fracPart = 0.0;
		double divisor = 1.0;

		for (;;)
			{
			int curDigit;
			if (not sTryRead_Digit(iStrimU, curDigit))
				break;
			divisor *= 10;
			fracPart *= 10;
			fracPart += curDigit;
			}
		oDouble += fracPart / divisor;
		}

	if (sTryRead_CP(iStrimU, 'e') || sTryRead_CP(iStrimU, 'E'))
		{
		oIsDouble = true;
		int64 exponent;
		if (not sTryRead_SignedDecimalInteger(iStrimU, exponent))
			spThrowParseException("Expected a valid exponent after 'e'");
		oDouble = oDouble * pow(10.0, int(exponent));
		}

	return true;
	}

bool sTryRead_SignedDecimalNumber(const ZStrimU& iStrimU,
	int64& oInt64, double& oDouble, bool& oIsDouble)
	{
	bool isNegative = false;
	bool hadSign = sTryRead_Sign(iStrimU, isNegative);
	if (sTryRead_DecimalNumber(iStrimU, oInt64, oDouble, oIsDouble))
		{
		if (isNegative)
			{
			oInt64 = -oInt64;
			oDouble = -oDouble;
			}
		return true;
		}

	if (hadSign)
		spThrowParseException("Expected a valid number after sign prefix");

	return false;
	}

bool sTryRead_Double(const ZStrimU& iStrimU, double& oDouble)
	{
	int64 dummyInt64;
	bool isDouble;
	return sTryRead_DecimalNumber(iStrimU, dummyInt64, oDouble, isDouble);
	}

bool sTryRead_SignedDouble(const ZStrimU& iStrimU, double& oDouble)
	{
	int64 dummyInt64;
	bool isDouble;
	return sTryRead_SignedDecimalNumber(iStrimU, dummyInt64, oDouble, isDouble);
	}

// -----------------

void sCopy_WS(const ZStrimU& iStrimU, const ChanW_UTF& oDest)
	{
	for (;;)
		{
		UTF32 theCP;
		if (not sQRead(theCP, iStrimU))
			break;
		if (not spIsWhitespace(theCP))
			{
			sUnread(theCP, iStrimU);
			break;
			}
		sWriteMust(theCP, oDest);
		}
	}

void sSkip_WS(const ZStrimU& iStrimU)
	{
	for (;;)
		{
		UTF32 theCP;
		if (not sQRead(theCP, iStrimU))
			break;
		if (not spIsWhitespace(theCP))
			{
			sUnread(theCP, iStrimU);
			break;
			}
		}
	}

// -----------------

void sCopy_WSAndCPlusPlusComments(const ZStrimU& iStrimU, const ChanW_UTF& oDest)
	{
	ZAssert(iStrimU.UnreadableLimit() >= 2);

	for (;;)
		{
		UTF32 firstCP;
		if (sQRead(firstCP, iStrimU))
			{
			if (spIsWhitespace(firstCP))
				{
				sWriteMust(firstCP, oDest);
				continue;
				}
			else if (firstCP == '/')
				{
				if (sTryRead_CP(iStrimU, '/'))
					{
					sWriteMust("//", oDest);
					sCopy_Line(iStrimU, oDest);
					continue;
					}
				else if (sTryRead_CP(iStrimU, '*'))
					{
					sWriteMust("/*", oDest);
					if (not sCopy_Until(iStrimU, "*/", oDest))
						spThrowParseException("Unexpected end of data while parsing a /**/ comment");
					sWriteMust("*/", oDest);
					continue;
					}
				}
			sUnread(firstCP, iStrimU);
			}
		break;
		}
	}

void sSkip_WSAndCPlusPlusComments(const ZStrimU& iStrimU)
	{ sCopy_WSAndCPlusPlusComments(iStrimU, ZStrimW_Null()); }


// -----------------

bool sCopy_Until(const ZStrimR& iStrimR,
	const string8& iTerminator, const ChanW_UTF& oDest)
	{
	ZStrimR_Boundary theStrimBoundary(iTerminator, iStrimR);
	sCopyAll(theStrimBoundary, oDest);
	return theStrimBoundary.HitBoundary();
	}

bool sSkip_Until(const ZStrimR& iStrimR, const string8& iTerminator)
	{ return sCopy_Until(iStrimR, iTerminator, ZStrimW_Null()); }

string8 sRead_Until(const ZStrimR& iStrimR, const string8& iTerminator)
	{
	string8 result;
	sCopy_Until(iStrimR, iTerminator, ChanW_UTF_string8(&result));
 	return result;
 	}

// -----------------

void sCopy_EscapedString(
	const ZStrimU& iStrimU, UTF32 iTerminator, const ChanW_UTF& oDest)
	{
	sCopyAll(ZStrimR_Escaped(iStrimU, iTerminator), oDest);
	}

void sRead_EscapedString(const ZStrimU& iStrimU, UTF32 iTerminator, string8& oString)
	{
	// Resize, rather than clear, so we don't discard any space reserved by our caller.
	oString.resize(0);
	sCopy_EscapedString(iStrimU, iTerminator, ChanW_UTF_string8(&oString));
	}

// -----------------

bool sTryCopy_EscapedString(const ZStrimU& iStrimU,
	UTF32 iDelimiter, const ChanW_UTF& oDest)
	{
	if (not sTryRead_CP(iStrimU, iDelimiter))
		return false;

	sCopy_EscapedString(iStrimU, iDelimiter, oDest);

	if (not sTryRead_CP(iStrimU, iDelimiter))
		spThrowParseException("Missing string delimiter");

	return true;
	}

bool sTryRead_EscapedString(const ZStrimU& iStrimU, UTF32 iDelimiter, string8& oString)
	{
	// Resize, rather than clear, so we don't discard any space reserved by our caller.
	oString.resize(0);
	return sTryCopy_EscapedString(iStrimU, iDelimiter, ChanW_UTF_string8(&oString));
	}

// -----------------

bool sTryCopy_Identifier(const ZStrimU& iStrimU, const ChanW_UTF& oDest)
	{
	UTF32 theCP;
	if (not sQRead(theCP, iStrimU))
		return false;

	if (not Unicode::sIsAlpha(theCP) && theCP != '_')
		{
		sUnread(theCP, iStrimU);
		return false;
		}

	sWriteMust(theCP, oDest);

	for (;;)
		{
		UTF32 theCP;
		if (not sQRead(theCP, iStrimU))
			break;

		if (not Unicode::sIsAlphaDigit(theCP) && theCP != '_')
			{
			sUnread(theCP, iStrimU);
			break;
			}
		sWriteMust(theCP, oDest);
		}

	return true;
	}

bool sTryRead_Identifier(const ZStrimU& iStrimU, string8& oString)
	{
	// Resize, rather than clear, so we don't discard any space reserved by our caller.
	oString.resize(0);
	return sTryCopy_Identifier(iStrimU, ChanW_UTF_string8(&oString));
	}


} // namespace ZUtil_Strim
} // namespace ZooLib