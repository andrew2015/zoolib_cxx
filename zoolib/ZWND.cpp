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

#include "zoolib/ZWND.h"

#if ZCONFIG_SPI_Enabled(Win)

#include "zoolib/ZDebug.h"
#include "zoolib/ZUnicode.h"

NAMESPACE_ZOOLIB_BEGIN

// =================================================================================================
#pragma mark -
#pragma mark * ZWNDW

void ZWNDW::sRegisterWindowClassW(const WCHAR* iWNDCLASSName, WNDPROC iWNDPROC)
	{
	WNDCLASSW windowClass;
	if (!::GetClassInfoW(::GetModuleHandleW(nullptr), iWNDCLASSName, &windowClass) &&
		!::GetClassInfoW(nullptr, iWNDCLASSName, &windowClass))
		{
		windowClass.style = 0;
		windowClass.lpfnWndProc = iWNDPROC;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = ::GetModuleHandleW(nullptr);
		windowClass.hIcon = nullptr;
		windowClass.hCursor = ::LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_ARROW));
		windowClass.hbrBackground = 0;//(HBRUSH)COLOR_WINDOW;
		windowClass.lpszMenuName = nullptr;
		windowClass.lpszClassName = iWNDCLASSName;
		ATOM theResult = ::RegisterClassW(&windowClass);
//		ZAssertStop(kDebug_Win, theResult != 0);
		}
	}

HWND ZWNDW::sCreateDefault(HWND iParent, DWORD iStyle, void* iCreateParam)
	{
	static const UTF16 sClassName[] = L"ZWNDW::DefWindowProc";
	sRegisterWindowClassW(sClassName, DefWindowProcW);

	return ::CreateWindowExW(
		0, // Extended attributes
		sClassName, // window class name
		nullptr, // window caption
		iStyle, // window style
		0, // initial x position
		0, // initial y position
		10, // initial x size
		10, // initial y size
		iParent, // Parent window
		nullptr,
		::GetModuleHandleW(nullptr), // program instance handle
		iCreateParam); // creation parameters
	}

ZWNDW::ZWNDW(WNDPROC iWNDPROC)
:	fWNDPROC(iWNDPROC),
	fHWND(nullptr)
	{}
	
ZWNDW::~ZWNDW()
	{
	if (HWND theHWND = fHWND)
		{
		fHWND = nullptr;
		::DestroyWindow(theHWND);
		}	
	}

void ZWNDW::Create(HWND iParent, DWORD iStyle)
	{
	ZAssert(!fHWND && fWNDPROC);

	static const UTF16 sClassName[] = L"ZWNDW::sWindowProcW";
	sRegisterWindowClassW(sClassName, sWindowProcW);

	HWND theHWND = ::CreateWindowExW(
		0, // Extended attributes
		sClassName, // window class name
		nullptr, // window caption
		iStyle, // window style
		0, // initial x position
		0, // initial y position
		10, // initial x size
		10, // initial y size
		iParent, // Parent window
		nullptr,
		::GetModuleHandleW(nullptr), // program instance handle
		this); // creation parameters

	ZAssert(fHWND == theHWND);
	}

HWND ZWNDW::GetHWND()
	{
	return fHWND;
	}

void ZWNDW::HWNDDestroyed()
	{
	ZAssert(!fHWND);
	delete this;
	}

LRESULT ZWNDW::WindowProc(HWND iHWND, UINT iMessage, WPARAM iWPARAM, LPARAM iLPARAM)
	{
	switch (iMessage)
		{
		case WM_NCDESTROY:
			{
			WNDPROC priorWNDPROC = fWNDPROC;
			if (HWND theHWND = fHWND)
				{
				fHWND = nullptr;
				this->HWNDDestroyed();
				}
			return ::CallWindowProcW(priorWNDPROC, iHWND, iMessage, iWPARAM, iLPARAM);
			}
		}
	return this->CallBase(iHWND, iMessage, iWPARAM, iLPARAM);
	}

LRESULT ZWNDW::CallBase(HWND iHWND, UINT iMessage, WPARAM iWPARAM, LPARAM iLPARAM)
	{
	return ::CallWindowProcW(fWNDPROC, iHWND, iMessage, iWPARAM, iLPARAM);
	}

LRESULT ZWNDW::sWindowProcW(HWND iHWND, UINT iMessage, WPARAM iWPARAM, LPARAM iLPARAM)
	{
	ZWNDW* theZWNDW = sFromHWNDNilOkayW(iHWND);

	if (!theZWNDW)
		{
		if (iMessage == WM_GETMINMAXINFO)
			{
			// The very first message sent to a window is (go figure)
			// WM_GETMINMAXINFO, so we have to handle it specially.
			return ::DefWindowProcW(iHWND, iMessage, iWPARAM, iLPARAM);
			}
		else if (iMessage == WM_NCCREATE)
			{
			// If this is the first message sent to a window, attach the ZWNDW to it
			CREATESTRUCTW* theCREATESTRUCT = reinterpret_cast<CREATESTRUCTW*>(iLPARAM);
			theZWNDW = reinterpret_cast<ZWNDW*>(theCREATESTRUCT->lpCreateParams);
			theZWNDW->fHWND = iHWND;
			// Attach the marker property
			::SetPropW(iHWND, L"ZWNDW", reinterpret_cast<HANDLE>(theZWNDW));
			// And set the user data
			::SetWindowLongPtrW(iHWND, GWLP_USERDATA, reinterpret_cast<LPARAM>(theZWNDW));
			}
		}

	ZAssertStop(0, theZWNDW);
	return theZWNDW->WindowProc(iHWND, iMessage, iWPARAM, iLPARAM);	
	}

ZWNDW* ZWNDW::sFromHWNDNilOkayW(HWND iHWND)
	{
	// Get the process ID of the window
	DWORD windowProcessID;
	::GetWindowThreadProcessId(iHWND, &windowProcessID);

	// And our own process ID
	DWORD currentProcessID = ::GetCurrentProcessId();
	if (windowProcessID == currentProcessID)
		{
		// This is an HWND in this process. Check for the marker property.
		if (::GetPropW(iHWND, L"ZWNDW"))
			{
			if (ZWNDW* theZWNDW
				= reinterpret_cast<ZWNDW*>((LONG_PTR)::GetWindowLongPtrW(iHWND, GWLP_USERDATA)))
				{
				ZAssertStop(0, theZWNDW->fHWND == iHWND);
				return theZWNDW;
				}
			}
		}
	return nullptr;
	}

// =================================================================================================
#pragma mark -
#pragma mark * ZWNDSubClassW

ZWNDSubClassW::ZWNDSubClassW()
:	fHWND(nullptr),
	fWNDPROC(nullptr)
	{}

ZWNDSubClassW::~ZWNDSubClassW()
	{
	ZAssert(!fHWND && !fWNDPROC);
	}

void ZWNDSubClassW::Attach(HWND iHWND)
	{
	ZAssert(!fHWND && !fWNDPROC);
	fHWND = iHWND;
	::SetPropW(iHWND, L"ZWNDSubClassW", reinterpret_cast<HANDLE>(this));
	fWNDPROC = (WNDPROC)(LPARAM)::SetWindowLongPtrW(fHWND, GWLP_WNDPROC, (LPARAM)sWindowProcW);
	}

void ZWNDSubClassW::Detach()
	{
	ZAssert(fHWND && fWNDPROC);
	::SetPropW(fHWND, L"ZWNDSubClassW_WNDPROC_Prior", (HANDLE)fWNDPROC);
	::SetPropW(fHWND, L"ZWNDSubClassW", nullptr);
	fHWND = nullptr;
	fWNDPROC = nullptr;
	}

HWND ZWNDSubClassW::GetHWND()
	{ return fHWND; }

void ZWNDSubClassW::HWNDDestroyed()
	{
	this->Detach();
	delete this;
	}

LRESULT ZWNDSubClassW::WindowProc(HWND iHWND, UINT iMessage, WPARAM iWPARAM, LPARAM iLPARAM)
	{
	switch (iMessage)
		{
		case WM_NCDESTROY:
			{
			WNDPROC priorWNDPROC = fWNDPROC;
			this->HWNDDestroyed();
			return ::CallWindowProcW(priorWNDPROC, iHWND, iMessage, iWPARAM, iLPARAM);
			}
		}
	return this->CallBase(iHWND, iMessage, iWPARAM, iLPARAM);
	}

LRESULT ZWNDSubClassW::CallBase(HWND iHWND, UINT iMessage, WPARAM iWPARAM, LPARAM iLPARAM)
	{
	ZAssert(fHWND && fWNDPROC);
	return ::CallWindowProcW(fWNDPROC, iHWND, iMessage, iWPARAM, iLPARAM);
	}

LRESULT CALLBACK ZWNDSubClassW::sWindowProcW(
	HWND iHWND, UINT iMessage, WPARAM iWPARAM, LPARAM iLPARAM)
	{
	if (ZWNDSubClassW* theZWND = sFromHWNDNilOkayW(iHWND))
		{
		return theZWND->WindowProc(iHWND, iMessage, iWPARAM, iLPARAM);
		}
	else
		{
		WNDPROC priorWNDPROC = (WNDPROC)::GetPropW(iHWND, L"ZWNDSubClassW_WNDPROC_Prior");
		return ::CallWindowProcW(priorWNDPROC, iHWND, iMessage, iWPARAM, iLPARAM);
		}
	}

ZWNDSubClassW* ZWNDSubClassW::sFromHWNDNilOkayW(HWND iHWND)
	{
	// Get the process ID of the window
	DWORD windowProcessID;
	::GetWindowThreadProcessId(iHWND, &windowProcessID);

	// And our own process ID
	DWORD currentProcessID = ::GetCurrentProcessId();
	if (windowProcessID == currentProcessID)
		{
		if (ZWNDSubClassW* theZWND
			= reinterpret_cast<ZWNDSubClassW*>(::GetPropW(iHWND, L"ZWNDSubClassW")))
			{
			ZAssertStop(0, theZWND->fHWND == iHWND);
			return theZWND;
			}
		}
	return nullptr;
	}

NAMESPACE_ZOOLIB_END

#endif // ZCONFIG_SPI_Enabled(Win)
