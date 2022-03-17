//
// Registry.cpp
// Contains the implementation of the exported functions that are
// called by regsvr32.exe to register / unregister the COM component.
//
#include "..\common\pch.h"
#include "atlbase.h"
#include "AddObj.h"
#include "AddObjFactory.h"
#include "IAdd_i.c" // Contains the interface IIDs

#define DEBUG_LOGGER_ENABLED
#define FILE_LOGGER_ENABLED
#define LOG_PREFIX "[ADDOBJ-EXPORTS]"
#include "logger.h"

CComModule _Module;

STDAPI DllRegisterServer(void)
{
	LOG("Entering");
	// registers object, typelib and all interfaces in typelib
	HRESULT hr = _Module.DllRegisterServer();
	LOG("CComModule.DllRegisterServer() returned 0x%08x", hr);
	if (FAILED(hr))
		return hr;
	//hr = RegisterAmsiProvider();
	return hr;
}

STDAPI DllUnregisterServer(void)
{
	LOG("Entering");
	HRESULT hr = _Module.DllUnregisterServer();
	LOG("CComModule.DllUnregisterServer() returned 0x%08x", hr);
	return hr;
}
