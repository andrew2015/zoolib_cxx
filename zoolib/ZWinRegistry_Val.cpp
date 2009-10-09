/* -------------------------------------------------------------------------------------------------
Copyright (c) 2009 Andrew Green
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

#include "zoolib/ZWinRegistry_Val.h"

#if ZCONFIG_SPI_Enabled(Win)

#include "zoolib/ZByteSwap.h"
#include "zoolib/ZUnicode.h"

NAMESPACE_ZOOLIB_BEGIN

namespace ZWinRegistry {

// =================================================================================================
#pragma mark -
#pragma mark * ZWinRegistry

static HKEY spClone(HKEY iHKEY)
	{
	if (iHKEY)
		{
		HKEY result;
		if (::DuplicateHandle(
			::GetCurrentProcess(), // hSourceProcessHandle
			iHKEY, // hSourceHandle
			::GetCurrentProcess(), // hTargetProcessHandle
			(LPHANDLE)&result, // lpTargetHandle
			0, // dwDesiredAccess
			false, // bInheritHandle
			DUPLICATE_SAME_ACCESS // dwOptions
			))
			{
			return result;
			}

		if (0x80000000 & (ULONG_PTR)iHKEY)
			return iHKEY;
		}
	return nullptr;
	}

static void spClose(HKEY iHKEY)
	{
	if (iHKEY)
		::RegCloseKey(iHKEY);
	}

static bool spCount(HKEY iHKEY,
	DWORD* oCountKeys, DWORD* oMaxLengthKeyName,
	DWORD* oCountValues, DWORD* oMaxLengthValueName)
	{
	if (iHKEY)
		{
		if (ERROR_SUCCESS == ::RegQueryInfoKeyW(
			iHKEY,
			nullptr, // lpClass
			nullptr, // lpcClass
			nullptr, // lpReserved,
			oCountKeys,
			oMaxLengthKeyName,
			nullptr, //  lpcMaxClassLen,
			oCountValues,
			oMaxLengthValueName,
			nullptr, // lpcMaxValueLen,
			nullptr, // lpcbSecurityDescriptor,
			nullptr // lpftLastWriteTime
			))
			{
			if (oMaxLengthKeyName)
				++*oMaxLengthKeyName;
			if (oMaxLengthValueName)
				++*oMaxLengthValueName;
			return true;
			}
		}
	return false;
	}

static string16 spReadLimited(const UTF16* iSource, size_t iLimit)
	{
	string16 result;
	while (iLimit--)
		{
		if (UTF32 theCP = ZUnicode::sReadInc(iSource))
			result += theCP;
		else
			break;
		}
	return result;
	}

static bool spQGet(DWORD iType, const void* iBuffer, DWORD iLength, Val& oVal)
	{
	switch (iType)
		{
		case REG_NONE:
			{
			oVal = Val();
			return true;
			}
		case REG_BINARY:
			{
			oVal = Data(iBuffer, iLength);
			return true;
			}
		case REG_DWORD_LITTLE_ENDIAN:
			{
			if (iLength == 4)
				{
				oVal = int32(ZByteSwap_ReadLittle32(iBuffer));
				return true;
				}
			break;
			}
		case REG_DWORD_BIG_ENDIAN:
			{
			if (iLength == 4)
				{
				oVal = int32(ZByteSwap_ReadBig32(iBuffer));
				return true;
				}
			break;
			}
		case REG_QWORD_LITTLE_ENDIAN:
			{
			if (iLength == 8)
				{
				oVal = int64(ZByteSwap_ReadLittle64(iBuffer));
				return true;
				}
			break;
			}
		case REG_EXPAND_SZ:
		case REG_LINK:
		case REG_SZ:
			{
			if (0 == (iLength & 1))
				{
				const string16 asString16 =
					spReadLimited(static_cast<const UTF16*>(iBuffer), iLength / 2);

				if (REG_EXPAND_SZ == iType)
					{
					oVal = String_Env(asString16);
					}
				else if (REG_LINK == iType)
					{
					oVal = String_Link(asString16);
					}
				else
					{
					oVal = asString16;
					}
				return true;
				}
			break;
			}
		case REG_MULTI_SZ:
			{
			vector<string16> theVec;
			for (const WCHAR* cur = static_cast<const WCHAR*>(iBuffer);
				*cur; /*no inc*/)
				{
				const string16 curString(cur);
				theVec.push_back(curString);
				cur += curString.length() + 1;
				}

			oVal = theVec;
			return true;
			}
		}
	return false;
	}

static string8 spKeyName(HKEY iHKEY, DWORD iMaxLengthKeyName, size_t iIndex)
	{
	vector<WCHAR> theName(iMaxLengthKeyName);
	if (ERROR_SUCCESS == ::RegEnumKeyExW(
		iHKEY,
		iIndex,
		&theName[0],
		&iMaxLengthKeyName,
		nullptr, // lpReserved
		nullptr, // lpClass
		nullptr, // lpcClass
		nullptr // lpftLastWriteTime
		))
		{
		return ZUnicode::sAsUTF8(spReadLimited(&theName[0], iMaxLengthKeyName));
		}
	return string8();
	}

static string8 spValueName(HKEY iHKEY, DWORD iMaxLengthValueName, size_t iIndex)
	{
	vector<WCHAR> theName(iMaxLengthValueName);
	if (ERROR_SUCCESS == ::RegEnumValueW(
		iHKEY,
		iIndex,
		&theName[0],
		&iMaxLengthValueName,
		nullptr, // lpReserved
		nullptr, // lpType,
		nullptr, // lpData,
		nullptr // lpcbData
		))
		{
		return ZUnicode::sAsUTF8(spReadLimited(&theName[0], iMaxLengthValueName));
		}
	return string8();
	}

// =================================================================================================
#pragma mark -
#pragma mark * ZWinRegistry::Val

ZAny Val::AsAny() const
	{ return *this; }

Val::Val()
	{}

Val::Val(const Val& iOther)
:	inherited(static_cast<const ZAny&>(iOther))
	{}

Val::~Val()
	{}

Val& Val::operator=(const Val& iOther)
	{
	inherited::operator=(static_cast<const ZAny&>(iOther));
	return *this;
	}

Val::Val(const string8& iVal)
:	inherited(ZUnicode::sAsUTF16(iVal))
	{}

Val::Val(const string16& iVal)
:	inherited(iVal)
	{}

Val::Val(const vector<string16>& iVal)
:	inherited(iVal)
	{}

Val::Val(const String_Env& iVal)
:	inherited(iVal)
	{}

Val::Val(const String_Link& iVal)
:	inherited(iVal)
	{}

Val::Val(const int32& iVal)
:	inherited(iVal)
	{}

Val::Val(const int64& iVal)
:	inherited(iVal)
	{}

Val::Val(const Data& iVal)
:	inherited(iVal)
	{}

Val::Val(const KeyRef& iVal)
:	inherited(iVal)
	{}

//ZMACRO_ZValAccessors_Def_GetP(,Val, String8, string8)
ZMACRO_ZValAccessors_Def_GetP(,Val, String16, string16)
ZMACRO_ZValAccessors_Def_GetP(,Val, StringList, vector<string16>)
ZMACRO_ZValAccessors_Def_GetP(,Val, String_Env, String_Env)
ZMACRO_ZValAccessors_Def_GetP(,Val, String_Link, String_Link)
ZMACRO_ZValAccessors_Def_GetP(,Val, Int32, int32)
ZMACRO_ZValAccessors_Def_GetP(,Val, Int64, int64)
ZMACRO_ZValAccessors_Def_GetP(,Val, Data, Data)
ZMACRO_ZValAccessors_Def_GetP(,Val, KeyRef, KeyRef)

// =================================================================================================
#pragma mark -
#pragma mark * ZWinRegistry::KeyRef

KeyRef::operator operator_bool_type() const
	{ return operator_bool_generator_type::translate(fHKEY); }

void KeyRef::swap(KeyRef& rhs)
	{ std::swap(fHKEY, rhs.fHKEY); }

KeyRef::KeyRef()
:	fHKEY(nullptr)
	{}

KeyRef::KeyRef(const KeyRef& iOther)
:	fHKEY(spClone(iOther.fHKEY))
	{}

KeyRef::~KeyRef()
	{ spClose(fHKEY); }

KeyRef& KeyRef::operator=(const KeyRef& iOther)
	{
	if (fHKEY != iOther.fHKEY)
		{
		spClose(fHKEY);
		fHKEY = spClone(iOther.fHKEY);
		}
	return *this;
	}

KeyRef::KeyRef(HKEY iOther)
:	fHKEY(spClone(iOther))
	{}

KeyRef& KeyRef::operator=(HKEY iOther)
	{
	if (fHKEY != iOther)
		{
		spClose(fHKEY);
		fHKEY = spClone(iOther);
		}
	return *this;
	}

KeyRef::KeyRef(const Adopt_T<HKEY>& iOther)
:	fHKEY(iOther.Get())
	{}

KeyRef& KeyRef::operator=(const Adopt_T<HKEY>& iOther)
	{
	HKEY otherHKEY = iOther.Get();
	if (fHKEY != otherHKEY)
		{
		spClose(fHKEY);
		fHKEY = otherHKEY;
		}
	return *this;
	}


bool KeyRef::QGet(const string16& iName, Val& oVal) const
	{
	if (iName.empty())
		return false;

	if (iName[0] != '!')
		{
		HKEY subKey;
		if (ERROR_SUCCESS == ::RegOpenKeyExW(
			fHKEY,
			iName.c_str(),
			0, // ulOptions
			KEY_READ,
			&subKey))
			{
			oVal = KeyRef(Adopt(subKey));
			return true;
			}
		return false;
		}

	DWORD curLength = 1024;
	for (;;)
		{
		DWORD type;
		vector<BYTE> bufferVec(curLength);
		DWORD length = curLength;
		LONG result = ::RegQueryValueExW(
			fHKEY,
			&iName.c_str()[1],
			nullptr, // lpReserved
			&type,
			&bufferVec[0],
			&length);

		if (ERROR_SUCCESS == result)
			return spQGet(type, &bufferVec[0], length, oVal);

		if (ERROR_MORE_DATA == result)
			{
			// If we're hitting HKEY_PERFORMANCE_DATA then the value of length is not
			// useful, the next call may need more space. So we just double curLength.
			curLength *= 2;
			continue;
			}

		return false;
		}
	}


bool KeyRef::QGet(const string8& iName, Val& oVal) const
	{ return this->QGet(ZUnicode::sAsUTF16(iName), oVal); }

bool KeyRef::QGet(const Index_t& iIndex, Val& oVal) const
	{
	DWORD countKeys, maxLengthKeyName, countValues, maxLengthValueName;
	if (!spCount(fHKEY, &countKeys, &maxLengthKeyName, &countValues, &maxLengthValueName))
		return false;

	size_t offset = iIndex;
	if (offset < countKeys)
		{
		vector<WCHAR> theName(maxLengthKeyName);
		if (ERROR_SUCCESS == ::RegEnumKeyExW(
			fHKEY,
			offset,
			&theName[0],
			&maxLengthKeyName,
			nullptr, // lpReserved
			nullptr, // lpClass
			nullptr, // lpcClass
			nullptr // lpftLastWriteTime
			))
			{
			HKEY subKey;
			if (ERROR_SUCCESS == ::RegOpenKeyExW(
				fHKEY,
				&theName[0],
				0, // ulOptions
				KEY_READ,
				&subKey))
				{
				oVal = KeyRef(Adopt(subKey));
				return true;
				}
			}
		return false;
		}

	offset -= countKeys;
	if (offset >= countValues)
		return false;
		
	vector<WCHAR> theName(maxLengthValueName);
	DWORD curLength = 1024;
	for (;;)
		{
		DWORD nameLength = theName.size();
		DWORD type;
		vector<BYTE> bufferVec(curLength);
		DWORD length = curLength;
		LONG result = ::RegEnumValueW(
			fHKEY,
			offset,
			&theName[0], // lpValueName
			&nameLength, // lpcbValueName
			nullptr, // lpReserved
			&type,
			&bufferVec[0],
			&length);

		if (ERROR_SUCCESS == result)
			return spQGet(type, &bufferVec[0], length, oVal);

		if (ERROR_MORE_DATA == result)
			{
			// See comment in other QGet method.
			curLength *= 2;
			continue;
			}

		return false;
		}
	}

Val KeyRef::DGet(const Val& iDefault, const string16& iName) const
	{
	Val result;
	if (this->QGet(iName, result))
		return result;
	return iDefault;
	}

Val KeyRef::DGet(const Val& iDefault, const string8& iName) const
	{
	Val result;
	if (this->QGet(ZUnicode::sAsUTF16(iName), result))
		return result;
	return iDefault;
	}

Val KeyRef::DGet(const Val& iDefault, const Index_t& iIndex) const
	{
	Val result;
	if (this->QGet(iIndex, result))
		return result;
	return iDefault;
	}

Val KeyRef::Get(const string16& iName) const
	{ return this->DGet(Val(), iName); }

Val KeyRef::Get(const string8& iName) const
	{ return this->DGet(Val(), ZUnicode::sAsUTF16(iName)); }

Val KeyRef::Get(const Index_t& iIndex) const
	{ return this->DGet(Val(), iIndex); }

//void KeyRef::Set(const string16& iName, const Val& iVal);
//void KeyRef::Set(const string8& iName, const Val& iVal);
//void KeyRef::Set(const Index_t& iIndex, const Val& iVal);

//void KeyRef::Erase(const string16& iName);
//void KeyRef::Erase(const string8& iName);
//void KeyRef::Erase(const Index_t& iIndex);

KeyRef::Index_t KeyRef::Begin() const
	{ return 0; }

KeyRef::Index_t KeyRef::End() const
	{
	DWORD countKeys, countValues;
	if (spCount(fHKEY, &countKeys, nullptr, &countValues, nullptr))
		return countKeys + countValues;
	return 0;
	}

string8 KeyRef::NameOf(const Index_t& iIndex) const
	{
	DWORD countKeys, maxLengthKeyName, countValues, maxLengthValueName;
	if (spCount(fHKEY, &countKeys, &maxLengthKeyName, &countValues, &maxLengthValueName))
		{
		size_t offset = iIndex;
		if (offset < countKeys)
			return spKeyName(fHKEY, maxLengthKeyName, offset);

		offset -= countKeys;
		if (offset < countValues)
			return "!" + spValueName(fHKEY, maxLengthValueName, offset);
		}
	return string8();
	}

KeyRef::Index_t KeyRef::IndexOf(const string8& iName) const
	{
	const Index_t end = this->End();
	for (Index_t x = this->Begin(); x != end; ++x)
		{
		if (iName == this->NameOf(x))
			return x;
		}
	return end;
	}

KeyRef::Index_t KeyRef::IndexOf(const KeyRef& iOther, const Index_t& iOtherIndex) const
	{ return iOtherIndex; }

} // namespace ZWinRegistry

NAMESPACE_ZOOLIB_END

#endif // ZCONFIG_SPI_Enabled(Win)
