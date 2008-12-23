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

#ifndef __ZRGBColor__
#define __ZRGBColor__ 1
#include "zconfig.h"
#include "zoolib/ZCONFIG_SPI.h"

#include "zoolib/ZTypes.h"

#if ZCONFIG_SPI_Enabled(QuickDraw)
#	include ZMACINCLUDE3(ApplicationServices,QD,QuickDraw.h)
#endif

#if ZCONFIG_SPI_Enabled(GDI)
	typedef unsigned long COLORREF;
#endif

#if ZCONFIG_SPI_Enabled(X11)
#	include "zoolib/ZCompat_Xlib.h"
#endif

#if ZCONFIG_SPI_Enabled(BeOS)
#	include <GraphicsDefs.h>
#endif

NAMESPACE_ZOOLIB_BEGIN

// =================================================================================================
#pragma mark -
#pragma mark * ZRGBColorPOD

// AG 98-05-27. We need a POD version of ZRGBColor in cases where we're embedding
// inside a union. A POD version has no constructor/destructor etc, and hence a bitwise
// copy is all that's gonna be done with it, which makes unions safe.

class ZRGBColorPOD
	{
public:
	static const ZRGBColorPOD sBlack;
	static const ZRGBColorPOD sRed;
	static const ZRGBColorPOD sGreen;
	static const ZRGBColorPOD sBlue;
	static const ZRGBColorPOD sYellow;
	static const ZRGBColorPOD sCyan;
	static const ZRGBColorPOD sMagenta;
	static const ZRGBColorPOD sWhite;

	uint16 red;
	uint16 green;
	uint16 blue;
	uint16 alpha;

	#if ZCONFIG_SPI_Enabled(QuickDraw)
	operator RGBColor() const
		{
		RGBColor theRGB;
		theRGB.red = red;
		theRGB.green = green;
		theRGB.blue = blue;
		return theRGB;
		}
	#endif

	#if ZCONFIG_SPI_Enabled(X11)
	operator XColor() const
		{
		XColor theXColor;
		theXColor.red = red;
		theXColor.blue = blue;
		theXColor.green = green;
		theXColor.pixel = 0;
		return theXColor;
		}
	#endif

	#if ZCONFIG_SPI_Enabled(BeOS)
	operator rgb_color() const
		{
		rgb_color theRGB;
		theRGB.red = red >> 8;
		theRGB.green = green >> 8;
		theRGB.blue = blue >> 8;
		theRGB.alpha = alpha >> 8;
		return theRGB;
		}
	#endif

	float floatRed() const
		{ return red / 65535.0; }

	float floatGreen() const
		{ return green / 65535.0; }

	float floatBlue() const
		{ return blue / 65535.0; }

	float floatAlpha() const
		{ return alpha / 65535.0; }

	uint16 Luminance() const
		{ return uint16((uint32(red) + uint32(green) + uint32(blue)) / 3); }

	double NTSCLuminance() const
		{ return ((0.3 * red) + (0.59 * green) + (0.11 * blue)) / 65535.0; }

	bool operator==(const ZRGBColorPOD& other) const
		{ return red == other.red && green == other.green && blue == other.blue; }

	bool operator!=(const ZRGBColorPOD& other) const
		{ return ! (*this == other); }

	ZRGBColorPOD& operator*=(double iMultiplier);
	ZRGBColorPOD operator*(double iMultiplier) const
		{
		ZRGBColorPOD newColor(*this);
		newColor *= iMultiplier;
		return newColor;
		}

	ZRGBColorPOD& operator/=(double iDivisor);
	ZRGBColorPOD operator/(double iDivisor) const
		{
		ZRGBColorPOD newColor(*this);
		newColor /= iDivisor;
		return newColor;
		}

	ZRGBColorPOD& operator*=(const ZRGBColorPOD& other);
	ZRGBColorPOD operator*(const ZRGBColorPOD& other) const
		{
		ZRGBColorPOD newColor(*this);
		newColor *= other;
		return newColor;
		}

	ZRGBColorPOD& operator+=(const ZRGBColorPOD& other);
	ZRGBColorPOD operator+(const ZRGBColorPOD& other) const
		{
		ZRGBColorPOD newColor(*this);
		newColor += other;
		return newColor;
		}

	ZRGBColorPOD& operator-=(const ZRGBColorPOD& other);
	ZRGBColorPOD operator-(const ZRGBColorPOD& other) const
		{
		ZRGBColorPOD newColor(*this);
		newColor -= other;
		return newColor;
		}

	bool operator<(const ZRGBColorPOD& other) const
		{
		return red < other.red ||
			red == other.red && green < other.green ||
			red == other.red && green == other.green && blue < other.blue;
		}
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZRGBColorSmallPOD

class ZRGBColorSmallPOD
	{
public:
	static const ZRGBColorSmallPOD sBlack;
	static const ZRGBColorSmallPOD sRed;
	static const ZRGBColorSmallPOD sGreen;
	static const ZRGBColorSmallPOD sBlue;
	static const ZRGBColorSmallPOD sYellow;
	static const ZRGBColorSmallPOD sCyan;
	static const ZRGBColorSmallPOD sMagenta;
	static const ZRGBColorSmallPOD sWhite;

	uint8 red;
	uint8 green;
	uint8 blue;
	uint8 alpha;

	#if ZCONFIG_SPI_Enabled(QuickDraw)
	operator RGBColor() const
		{
		RGBColor theRGB;
		theRGB.red = uint16(red * 0x101);
		theRGB.green = uint16(green * 0x101);
		theRGB.blue = uint16(blue * 0x101);
		return theRGB;
		}
	#endif

	#if ZCONFIG_SPI_Enabled(X11)
	operator XColor() const
		{
		XColor theXColor;
		theXColor.red = red * 0x101;
		theXColor.green = green * 0x101;
		theXColor.blue = blue * 0x101;
		return theXColor;
		}
	#endif

	#if ZCONFIG_SPI_Enabled(BeOS)
	operator rgb_color() const
		{ return *((rgb_color*)this); }
	#endif

	float floatRed() const
		{ return red / 255.0; }

	float floatGreen() const
		{ return green / 255.0; }

	float floatBlue() const
		{ return blue / 255.0; }

	float floatAlpha() const
		{ return alpha / 255.0; }

	uint8 Luminance() const
		{ return uint8((uint16(red) + uint16(green) + uint16(blue)) / 3); }

	double NTSCLuminance() const
		{ return ((0.3 * red) + (0.59 * green) + (0.11 * blue)) / 255.0; }

	bool operator==(const ZRGBColorSmallPOD& other) const
		{ return red == other.red && green == other.green && blue == other.blue; }

	bool operator!=(const ZRGBColorSmallPOD& other) const
		{ return ! (*this == other); }

	ZRGBColorSmallPOD& operator*=(double iMultiplier);
	ZRGBColorSmallPOD operator*(double iMultiplier) const
		{
		ZRGBColorSmallPOD newColor(*this);
		newColor *= iMultiplier;
		return newColor;
		}

	ZRGBColorSmallPOD& operator/=(double iDivisor);
	ZRGBColorSmallPOD operator/(double iDivisor) const
		{
		ZRGBColorSmallPOD newColor(*this);
		newColor /= iDivisor;
		return newColor;
		}

	ZRGBColorSmallPOD& operator*=(const ZRGBColorSmallPOD& other);
	ZRGBColorSmallPOD operator*(const ZRGBColorSmallPOD& other) const
		{
		ZRGBColorSmallPOD newColor(*this);
		newColor *= other;
		return newColor;
		}

	ZRGBColorSmallPOD& operator+=(const ZRGBColorSmallPOD& other);
	ZRGBColorSmallPOD operator+(const ZRGBColorSmallPOD& other) const
		{
		ZRGBColorSmallPOD newColor(*this);
		newColor += other;
		return newColor;
		}

	ZRGBColorSmallPOD& operator-=(const ZRGBColorSmallPOD& other);
	ZRGBColorSmallPOD operator-(const ZRGBColorSmallPOD& other) const
		{
		ZRGBColorSmallPOD newColor(*this);
		newColor -= other;
		return newColor;
		}

	bool operator<(const ZRGBColorSmallPOD& other) const
		{
		return red < other.red ||
			red == other.red && green < other.green ||
			red == other.red && green == other.green && blue < other.blue;
		}
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZRGBColor

class ZRGBColor : public ZRGBColorPOD
	{
public:
	ZRGBColor(const ZRGBColorPOD& other)
		{
		red = other.red;
		green = other.green;
		blue = other.blue;
		alpha = other.alpha;
		}

	ZRGBColor& operator=(const ZRGBColorPOD& other)
		{
		red = other.red;
		green = other.green;
		blue = other.blue;
		alpha = other.alpha;
		return *this;
		}

	ZRGBColor(const ZRGBColorSmallPOD& other)
		{
		red = uint16(other.red * 0x101);
		green = uint16(other.green * 0x101);
		blue = uint16(other.blue * 0x101);
		alpha = uint16(other.alpha * 0x101);
		}

	ZRGBColor& operator=(const ZRGBColorSmallPOD& other)
		{
		red = uint16(other.red * 0x101);
		green = uint16(other.green * 0x101);
		blue = uint16(other.blue * 0x101);
		alpha = uint16(other.alpha * 0x101);
		return *this;
		}

	#if ZCONFIG_SPI_Enabled(QuickDraw)
	ZRGBColor(const RGBColor& iRGBColor)
		{
		red = iRGBColor.red;
		green = iRGBColor.green;
		blue = iRGBColor.blue;
		alpha = 0xFFFFU;
		}

	ZRGBColor& operator=(const RGBColor& iRGBColor)
		{
		red = iRGBColor.red;
		green = iRGBColor.green;
		blue = iRGBColor.blue;
		alpha = 0xFFFFU;
		return *this;
		}
	#endif

	#if ZCONFIG_SPI_Enabled(GDI)
	static ZRGBColor sFromCOLORREF(COLORREF iColorRef);
	#endif

	#if ZCONFIG_SPI_Enabled(X11)
	ZRGBColor(const XColor& iXColor)
		{
		red = iXColor.red;
		green = iXColor.green;
		blue = iXColor.blue;
		alpha = 0xFFFFU;
		}

	ZRGBColor& operator=(const XColor& iXColor)
		{
		red = iXColor.red;
		green = iXColor.green;
		blue = iXColor.blue;
		alpha = 0xFFFFU;
		return *this;
		}
	#endif

	#if ZCONFIG_SPI_Enabled(BeOS)
	ZRGBColor(const rgb_color& other)
		{
		red = uint16(other.red * 0x101);
		green = uint16(other.green * 0x101);
		blue = uint16(other.blue * 0x101);
		alpha = uint16(other.alpha * 0x101);
		return *this;
		}

	ZRGBColor& operator=(const rgb_color& other)
		{
		red = uint16(other.red * 0x101);
		green = uint16(other.green * 0x101);
		blue = uint16(other.blue * 0x101);
		alpha = uint16(other.alpha * 0x101);
		return *this;
		}
	#endif

	ZRGBColor() {}

	explicit ZRGBColor(uint16 iGray)
		{
		red = green = blue = iGray;
		alpha = 0xFFFFU;
		}

	ZRGBColor(uint16 iGray, uint16 iAlpha)
		{
		red = green = blue = iGray;
		alpha = iAlpha;
		}

	ZRGBColor(uint16 iRed, uint16 iGreen, uint16 iBlue)
		{
		red = iRed;
		green = iGreen;
		blue = iBlue;
		alpha = 0xFFFFU;
		}

	ZRGBColor(uint16 iRed, uint16 iGreen, uint16 iBlue, uint16 iAlpha)
		{
		red = iRed;
		green = iGreen;
		blue = iBlue;
		alpha = iAlpha;
		}

	explicit ZRGBColor(int iGray)
		{
		red = green = blue = uint16(iGray);
		alpha = 0xFFFFU;
		}

	ZRGBColor(int iGray, int iAlpha)
		{
		red = green = blue = uint16(iGray);
		alpha = uint16(iAlpha);
		}

	ZRGBColor(int iRed, int iGreen, int iBlue)
		{
		red = uint16(iRed);
		green = uint16(iGreen);
		blue = uint16(iBlue);
		alpha = 0xFFFFU;
		}

	ZRGBColor(int iRed, int iGreen, int iBlue, int iAlpha)
		{
		red = uint16(iRed);
		green = uint16(iGreen);
		blue = uint16(iBlue);
		alpha = uint16(iAlpha);
		}

	explicit ZRGBColor(unsigned int iGray)
		{
		red = green = blue = uint16(iGray);
		alpha = 0xFFFFU;
		}

	ZRGBColor(unsigned int iGray, unsigned int iAlpha)
		{
		red = green = blue = uint16(iGray);
		alpha = uint16(iAlpha);
		}

	ZRGBColor(unsigned int iRed, unsigned int iGreen, unsigned int iBlue)
		{
		red = uint16(iRed);
		green = uint16(iGreen);
		blue = uint16(iBlue);
		alpha = 0xFFFFU;
		}

	ZRGBColor(unsigned int iRed, unsigned int iGreen, unsigned int iBlue, unsigned int iAlpha)
		{
		red = uint16(iRed);
		green = uint16(iGreen);
		blue = uint16(iBlue);
		alpha = uint16(iAlpha);
		}

	explicit ZRGBColor(float iGray)
		{
		red = green = blue = uint16(iGray * 65535);
		alpha = 0xFFFFU;
		}

	ZRGBColor(float iGray, float iAlpha)
		{
		red = green = blue = uint16(iGray * 65535);
		alpha = 0xFFFFU;
		}

	ZRGBColor(float iRed, float iGreen, float iBlue)
		{
		red = uint16(iRed * 65535);
		green = uint16(iGreen * 65535);
		blue = uint16(iBlue * 65535);
		alpha = 0xFFFFU;
		}

	ZRGBColor(float iRed, float iGreen, float iBlue, float iAlpha)
		{
		red = uint16(iRed * 65535);
		green = uint16(iGreen * 65535);
		blue = uint16(iBlue * 65535);
		alpha = uint16(iAlpha * 65535);
		}

	explicit ZRGBColor(double iGray)
		{
		red = green = blue = uint16(iGray * 65535);
		alpha = 0xFFFFU;
		}

	ZRGBColor(double iGray, double iAlpha)
		{
		red = green = blue = uint16(iGray * 65535);
		alpha = 0xFFFFU;
		}

	ZRGBColor(double iRed, double iGreen, double iBlue)
		{
		red = uint16(iRed * 65535);
		green = uint16(iGreen * 65535);
		blue = uint16(iBlue * 65535);
		alpha = 0xFFFFU;
		}

	ZRGBColor(double iRed, double iGreen, double iBlue, double iAlpha)
		{
		red = uint16(iRed * 65535);
		green = uint16(iGreen * 65535);
		blue = uint16(iBlue * 65535);
		alpha = uint16(iAlpha * 65535);
		}
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZRGBColorSmall

class ZRGBColorSmall : public ZRGBColorSmallPOD
	{
public:
	ZRGBColorSmall(const ZRGBColorSmallPOD& other)
		{
		red = other.red;
		green = other.green;
		blue = other.blue;
		alpha = other.alpha;
		}

	ZRGBColorSmall& operator=(const ZRGBColorSmallPOD& other)
		{
		red = other.red;
		green = other.green;
		blue = other.blue;
		alpha = other.alpha;
		return *this;
		}

	ZRGBColorSmall(const ZRGBColorPOD& other)
		{
		red = uint8(other.red >> 8);
		green = uint8(other.green >> 8);
		blue = uint8(other.blue >> 8);
		alpha = uint8(other.alpha >> 8);
		}

	ZRGBColorSmall& operator=(const ZRGBColorPOD& other)
		{
		red = uint8(other.red >> 8);
		green = uint8(other.green >> 8);
		blue = uint8(other.blue >> 8);
		alpha = uint8(other.alpha >> 8);
		return *this;
		}

	#if ZCONFIG_SPI_Enabled(QuickDraw)
	ZRGBColorSmall(const RGBColor& iRGBColor)
		{
		red = uint8(iRGBColor.red >> 8);
		green = uint8(iRGBColor.green >> 8);
		blue = uint8(iRGBColor.blue >> 8);
		alpha = 0xFFU;
		}

	ZRGBColorSmall& operator=(const RGBColor& iRGBColor)
		{
		red = uint8(iRGBColor.red >> 8);
		green = uint8(iRGBColor.green >> 8);
		blue = uint8(iRGBColor.blue >> 8);
		alpha = 0xFFU;
		return *this;
		}
	#endif

	#if ZCONFIG_SPI_Enabled(GDI)
	static ZRGBColorSmall sFromCOLORREF(COLORREF iColorRef);
	#endif

	#if ZCONFIG_SPI_Enabled(X11)
	ZRGBColorSmall(const XColor& iXColor)
		{
		red = uint8(iXColor.red >> 8);
		green = uint8(iXColor.green >> 8);
		blue = uint8(iXColor.blue >> 8);
		alpha = 0xFFU;
		}

	ZRGBColorSmall& operator=(const XColor& iXColor)
		{
		red = uint8(iXColor.red >> 8);
		green = uint8(iXColor.green >> 8);
		blue = uint8(iXColor.blue >> 8);
		alpha = 0xFFU;
		return *this;
		}
	#endif

	#if ZCONFIG_SPI_Enabled(BeOS)
	ZRGBColorSmall(const rgb_color& other)
		{
		red = other.red;
		green = other.green;
		blue = other.blue;
		alpha = other.alpha;
		}

	ZRGBColorSmall& operator=(const rgb_color& other)
		{
		red = other.red;
		green = other.green;
		blue = other.blue;
		alpha = other.alpha;
		return *this;
		}
	#endif

	ZRGBColorSmall() {}

	explicit ZRGBColorSmall(uint8 iGray)
		{
		red = green = blue = iGray;
		alpha = 0xFFU;
		}

	ZRGBColorSmall(uint8 iGray, uint8 iAlpha)
		{
		red = green = blue = iGray;
		alpha = iAlpha;
		}

	ZRGBColorSmall(uint8 iRed, uint8 iGreen, uint8 iBlue)
		{
		red = iRed;
		green = iGreen;
		blue = iBlue;
		alpha = 0xFFU;
		}

	ZRGBColorSmall(uint8 iRed, uint8 iGreen, uint8 iBlue, uint8 iAlpha)
		{
		red = iRed;
		green = iGreen;
		blue = iBlue;
		alpha = iAlpha;
		}

	explicit ZRGBColorSmall(int iGray)
		{
		red = green = blue = uint8(iGray);
		alpha = 0xFFU;
		}

	ZRGBColorSmall(int iGray, int iAlpha)
		{
		red = green = blue = uint8(iGray);
		alpha = iAlpha;
		}

	ZRGBColorSmall(int iRed, int iGreen, int iBlue)
		{
		red = uint8(iRed);
		green = uint8(iGreen);
		blue = uint8(iBlue);
		alpha = 0xFFU;
		}

	ZRGBColorSmall(int iRed, int iGreen, int iBlue, int iAlpha)
		{
		red = uint8(iRed);
		green = uint8(iGreen);
		blue = uint8(iBlue);
		alpha = uint8(iAlpha);
		}

	explicit ZRGBColorSmall(unsigned int iGray)
		{
		red = green = blue = uint8(iGray);
		alpha = 0xFFU;
		}

	ZRGBColorSmall(unsigned int iGray, unsigned int iAlpha)
		{
		red = green = blue = uint8(iGray);
		alpha = iAlpha;
		}

	ZRGBColorSmall(unsigned int iRed, unsigned int iGreen, unsigned int iBlue)
		{
		red = uint8(iRed);
		green = uint8(iGreen);
		blue = uint8(iBlue);
		alpha = 0xFFU;
		}

	ZRGBColorSmall(unsigned int iRed, unsigned int iGreen, unsigned int iBlue, unsigned int iAlpha)
		{
		red = uint8(iRed);
		green = uint8(iGreen);
		blue = uint8(iBlue);
		alpha = uint8(iAlpha);
		}

	explicit ZRGBColorSmall(float iGray)
		{
		red = green = blue = uint16(iGray * 255);
		alpha = 0xFFU;
		}

	ZRGBColorSmall(float iGray, float iAlpha)
		{
		red = green = blue = uint16(iGray * 255);
		alpha = iAlpha;
		}

	ZRGBColorSmall(float iRed, float iGreen, float iBlue)
		{
		red = uint16(iRed * 255);
		green = uint16(iGreen * 255);
		blue = uint16(iBlue * 255);
		alpha = 0xFFU;
		}

	ZRGBColorSmall(float iRed, float iGreen, float iBlue, float iAlpha)
		{
		red = uint16(iRed * 255);
		green = uint16(iGreen * 255);
		blue = uint16(iBlue * 255);
		alpha = uint16(iAlpha * 255);
		}

	explicit ZRGBColorSmall(double iGray)
		{
		red = green = blue = uint16(iGray * 255);
		alpha = 0xFFU;
		}

	ZRGBColorSmall(double iGray, double iAlpha)
		{
		red = green = blue = uint16(iGray * 255);
		alpha = iAlpha;
		}

	ZRGBColorSmall(double iRed, double iGreen, double iBlue)
		{
		red = uint16(iRed * 255);
		green = uint16(iGreen * 255);
		blue = uint16(iBlue * 255);
		alpha = 0xFFU;
		}

	ZRGBColorSmall(double iRed, double iGreen, double iBlue, double iAlpha)
		{
		red = uint16(iRed * 255);
		green = uint16(iGreen * 255);
		blue = uint16(iBlue * 255);
		alpha = uint16(iAlpha * 255);
		}
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZRGBColorMap

class ZRGBColorMap
	{
public:
	uint8 fPixval;
	ZRGBColorPOD fColor;
	};

NAMESPACE_ZOOLIB_END

#endif // __ZRGBColor__
