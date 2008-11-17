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

#ifndef __ZNetscape_Guest_Std__
#define __ZNetscape_Guest_Std__ 1
#include "zconfig.h"

#include "zoolib/ZNetscape_Guest.h"

namespace ZNetscape {

class Guest_Std;

// =================================================================================================
#pragma mark -
#pragma mark * GuestMeister_Std

class GuestMeister_Std : public GuestMeister
	{
protected:
	GuestMeister_Std();

public:
	virtual ~GuestMeister_Std();
	
// From GuestMeister
	virtual NPError New(
		NPMIMEType pluginType, NPP instance, uint16 mode,
		int16 argc, char* argn[], char* argv[], NPSavedData* saved);

	virtual NPError Destroy(NPP instance, NPSavedData** save);

	virtual NPError SetWindow(NPP instance, NPWindow* window);

	virtual NPError NewStream(NPP instance,
		NPMIMEType type, NPStream* stream, NPBool seekable, uint16* stype);

	virtual NPError DestroyStream(NPP instance, NPStream* stream, NPReason reason);

	virtual int32 WriteReady(NPP instance, NPStream* stream);

	virtual int32 Write(NPP instance, NPStream* stream, int32 offset, int32 len, void* buffer);

	virtual void StreamAsFile(NPP instance, NPStream* stream, const char* fname);

	virtual void Print(NPP instance, NPPrint* platformPrint);

	virtual int16 HandleEvent(NPP instance, void* event);

	virtual void URLNotify(NPP instance, const char* url, NPReason reason, void* notifyData);

	virtual jref GetJavaClass();

	virtual NPError GetValue(NPP instance, NPPVariable variable, void *value);

	virtual NPError SetValue(NPP instance, NPNVariable variable, void *value);

// Our protocol
	virtual Guest_Std* MakeGuest(
		NPMIMEType pluginType, NPP instance, uint16 mode,
		int16 argc, char* argn[], char* argv[], NPSavedData* saved) = 0;
	};

// =================================================================================================
#pragma mark -
#pragma mark * Guest_Std

class Guest_Std : public Guest
	{
protected:
	Guest_Std(NPP iNPP);

public:
	virtual ~Guest_Std();
	
// Our protocol
	virtual NPError Guest_Destroy(NPSavedData** save);

	virtual NPError Guest_SetWindow(NPWindow* window);

	virtual NPError Guest_NewStream(
		NPMIMEType type, NPStream* stream, NPBool seekable, uint16* stype);

	virtual NPError Guest_DestroyStream(NPStream* stream, NPReason reason);

	virtual int32 Guest_WriteReady(NPStream* stream);

	virtual int32 Guest_Write(NPStream* stream, int32 offset, int32 len, void* buffer);

	virtual void Guest_StreamAsFile(NPStream* stream, const char* fname);

	virtual void Guest_Print(NPPrint* platformPrint);

	virtual int16 Guest_HandleEvent(void* event);

	virtual void Guest_URLNotify(const char* url, NPReason reason, void* notifyData);

	virtual NPError Guest_GetValue(NPPVariable iNPPVariable, void* oValue);

	virtual NPError Guest_SetValue(NPNVariable iNPNVariable, void* iValue);
	};

} // namespace ZNetscape

#endif // __ZNetscape_Guest__
