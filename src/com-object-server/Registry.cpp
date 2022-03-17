//
// Registry.cpp
// Contains the implementation of the exported functions that are
// called by regsvr32.exe to register / unregister the COM component.
//
#include "..\common\pch.h"
#include "AddObj.h"
#include "AddObjFactory.h"
#include "IAdd_i.c" // Contains the interface IIDs

#define DEBUG_LOGGER_ENABLED
#define FILE_LOGGER_ENABLED
#define LOG_PREFIX "[ADDOBJ-EXPORTS]"
#include "logger.h"

static const TCHAR gc_szClassDescription[] = TEXT("SuperFast Addition COM Object");
static const TCHAR gc_szBoth[] = TEXT("Both");
static const TCHAR gc_szThreadingModel[] = TEXT("ThreadingModel");
static const TCHAR gc_szInProcServer[] = TEXT("InProcServer32");

STDAPI DllRegisterServer(void)
{
    HRESULT hr = S_OK;
    LONG lRet = ERROR_SUCCESS;
    HKEY hClsidKey = NULL;
    HKEY hInProcKey = NULL;
    LPOLESTR lpszGuid = NULL;
    TCHAR szRegKey[45] = { 0 };
    TCHAR szFileName[MAX_PATH] = { 0 };

    LOG("Entering");

    CoInitialize(NULL);

    lRet = GetModuleFileName(g_hModule, szFileName, sizeof(szFileName));
    if (lRet == 0) {
        hr = HRESULT_FROM_WIN32(GetLastError());
        goto done;
    }

    hr = StringFromCLSID(CLSID_AddObject, &lpszGuid);
    if (FAILED(hr))
        goto done;

    hr = StringCbPrintf(szRegKey, sizeof(szRegKey), TEXT("CLSID\\%s"), lpszGuid);
    if (FAILED(hr))
        goto done;

    lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT, szRegKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE | KEY_CREATE_SUB_KEY, NULL, &hClsidKey, NULL);
    if (lRet != ERROR_SUCCESS) {
        hr = HRESULT_FROM_WIN32(lRet);
        goto done;
    }

    lRet = RegSetValueEx(hClsidKey, NULL, 0, REG_SZ, (PBYTE)gc_szClassDescription, sizeof(gc_szClassDescription));
    if (lRet != ERROR_SUCCESS) {
        hr = HRESULT_FROM_WIN32(lRet);
        goto done;
    }

    lRet = RegCreateKeyEx(hClsidKey, gc_szInProcServer, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hInProcKey, NULL);
    if (lRet != ERROR_SUCCESS) {
        hr = HRESULT_FROM_WIN32(lRet);
        goto done;
    }

    lRet = RegSetValueEx(hInProcKey, NULL, 0, REG_SZ, (PBYTE)szFileName, sizeof(szFileName));
    if (lRet != ERROR_SUCCESS) {
        hr = HRESULT_FROM_WIN32(lRet);
        goto done;
    }

    lRet = RegSetValueEx(hInProcKey, gc_szThreadingModel, 0, REG_SZ, (PBYTE)gc_szBoth, sizeof(gc_szBoth));
    if (lRet != ERROR_SUCCESS) {
        hr = HRESULT_FROM_WIN32(lRet);
        goto done;
    }

done:
    if (hInProcKey != NULL)
        RegCloseKey(hInProcKey);

    if (hClsidKey != NULL)
        RegCloseKey(hClsidKey);

    if (lpszGuid != NULL)
        CoTaskMemFree(lpszGuid);

    CoUninitialize();

    LOG("Exiting with 0x%08x", hr);

    return hr;
}

STDAPI DllUnregisterServer(void)
{
    HRESULT hr = S_OK;
    LONG lRet = ERROR_SUCCESS;
    HKEY hClsidKey = NULL;
    LPOLESTR lpszGuid = NULL;

    LOG("Entering");

    CoInitialize(NULL);

    hr = StringFromCLSID(CLSID_AddObject, &lpszGuid);
    if (FAILED(hr))
        goto done;

    lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT, TEXT("CLSID"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hClsidKey, NULL);
    if (lRet != ERROR_SUCCESS) {
        hr = HRESULT_FROM_WIN32(lRet);
        goto done;
    }

    lRet = RegDeleteTree(hClsidKey, lpszGuid);
    if (lRet != ERROR_SUCCESS) {
        hr = HRESULT_FROM_WIN32(lRet);
        goto done;
    }

done:
    if (hClsidKey != NULL)
        RegCloseKey(hClsidKey);

    if (lpszGuid != NULL)
        CoTaskMemFree(lpszGuid);

    CoUninitialize();

    LOG("Exiting with 0x%08x", hr);

    return hr;
}
