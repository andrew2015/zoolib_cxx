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

#ifndef __ZYad_CFType__
#define __ZYad_CFType__ 1
#include "zconfig.h"

#include "zoolib/ZCONFIG_SPI.h"

#include "zoolib/ZYad.h"

#if ZCONFIG_SPI_Enabled(CFType)

#include "zoolib/ZStream_CFData.h"

#include <CoreFoundation/CFArray.h>
#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFDictionary.h>

#include <vector>

NAMESPACE_ZOOLIB_BEGIN

// =================================================================================================
#pragma mark -
#pragma mark * ZYadR_CFType

class ZYadR_CFType : public virtual ZYadR
	{
public:
	ZYadR_CFType(CFTypeRef iCFTypeRef);
	virtual ~ZYadR_CFType();

// Our protocol
	CFTypeRef GetCFTypeRef();

private:	
	ZRef<CFTypeRef> fCFTypeRef;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZYadPrimR_CFType

class ZYadPrimR_CFType
:	public virtual ZYadR_CFType,
	public virtual ZYadPrimR
	{
public:
	ZYadPrimR_CFType(CFTypeRef iCFTypeRef);
	virtual ~ZYadPrimR_CFType();

// From ZYadR via ZYadPrimR
	virtual bool IsSimple(const ZYadOptions& iOptions);
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZYadRawRPos_CFData

typedef ZStreamerRPos_T<ZStreamRPos_CFData> ZStreamerRPos_CFData;

class ZYadRawRPos_CFData
:	public virtual ZYadR_CFType,
	public virtual ZYadRawR,
	public virtual ZStreamerRPos_CFData
	{
public:
	ZYadRawRPos_CFData(CFDataRef iCFDataRef);
	virtual ~ZYadRawRPos_CFData();
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZYadListRPos_CFArray

class ZYadListRPos_CFArray
:	public virtual ZYadR_CFType,
	public virtual ZYadListRPos
	{
public:
	ZYadListRPos_CFArray(CFArrayRef iCFArrayRef);
	ZYadListRPos_CFArray(CFArrayRef iCFArrayRef, uint64 iPosition);

// From ZYadR via ZYadListRPos
	virtual bool HasChild();
	virtual ZRef<ZYadR> NextChild();

// From ZYadListR via ZYadListRPos
	virtual uint64 GetPosition();

// From ZYadListRPos
	virtual uint64 GetSize();
	virtual void SetPosition(uint64 iPosition);
	virtual ZRef<ZYadListRPos> Clone();

private:
	ZRef<CFArrayRef> fCFArrayRef;
	uint64 fPosition;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZYadMapRPos_CFDictionary

class ZYadMapRPos_CFDictionary
:	public virtual ZYadR_CFType,
	public virtual ZYadMapRPos
	{
	ZYadMapRPos_CFDictionary(CFDictionaryRef iCFDictionaryRef,
		uint64 iPosition,
		const std::vector<CFStringRef>& iNames,
		const std::vector<CFTypeRef>& iValues);

public:
	ZYadMapRPos_CFDictionary(CFDictionaryRef iCFDictionaryRef);

// From ZYadR via ZYadMapRPos
	virtual bool HasChild();
	virtual ZRef<ZYadR> NextChild();

// From ZYadMapR via ZYadMapRPos
	virtual std::string Name();

// From ZYadMapRPos
	virtual void SetPosition(const std::string& iName);
	virtual ZRef<ZYadMapRPos> Clone();

private:
	ZRef<CFDictionaryRef> fCFDictionaryRef;
	uint64 fPosition;
	std::vector<CFStringRef> fNames;
	std::vector<CFTypeRef> fValues;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZYad_CFType

namespace ZYad_CFType {

ZRef<ZYadR> sMakeYadR(CFTypeRef iCFTypeRef);

CFTypeRef sFromYadR(ZRef<ZYadR>);

} // namespace ZYad_CFType

NAMESPACE_ZOOLIB_END

#endif // ZCONFIG_SPI_Enabled(CFType)

#endif // __ZYad_ZooLib__
