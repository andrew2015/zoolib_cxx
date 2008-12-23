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

#include "zoolib/ZBlackBerryCOM.h"

#if ZCONFIG_SPI_Enabled(Win)

NAMESPACE_ZOOLIB_USING

namespace ZBlackBerryCOM {

ZWinCOM_DEFINITION(IChannelEvents)
ZWinCOM_DEFINITION(IChannel)
ZWinCOM_DEFINITION(IDeviceProperty)
ZWinCOM_DEFINITION(IDeviceProperties)
ZWinCOM_DEFINITION(IDevice)
ZWinCOM_DEFINITION(IDevices)
ZWinCOM_DEFINITION(IDeviceManagerEvents)
ZWinCOM_DEFINITION(IDeviceManagerNotification)
ZWinCOM_DEFINITION(IDeviceManager)
const GUID IDeviceManager::sCLSID =
	{0xBA3D0120,0xE617,0x4F66,{0xAD,0xCA,0x58,0x5C,0xC2,0xFB,0x86,0xDB}};

} // namespace ZBlackBerryCOM

#endif // ZCONFIG_SPI_Enabled(Win)
