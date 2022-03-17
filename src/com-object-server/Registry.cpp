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

static const TCHAR g_szClassDescription[] = TEXT("SuperFast Addition COM Object");
static const TCHAR g_szProgId[] = TEXT("SuperFast.AddObj");
static const TCHAR g_szBoth[] = TEXT("Both");
static const TCHAR g_szThreadingModel[] = TEXT("ThreadingModel");
static const TCHAR g_szInProcServer[] = TEXT("InProcServer32");
static const TCHAR g_szTypeLib[] = TEXT("TypeLib");
static const TCHAR g_szVersion[] = TEXT("Version");
static const TCHAR g_szVersionValue[] = TEXT("1.0");

#define OnErrorJump(hr, lRet, label)   \
    if (lRet != ERROR_SUCCESS) {       \
        hr = HRESULT_FROM_WIN32(lRet); \
        goto label;                    \
    }                                  \

STDAPI AddProgIdEntry(LPOLESTR szClassId)
{
    HRESULT hr = S_OK;
    LONG lRet = ERROR_SUCCESS;
    TCHAR szProgIdRegKey[60] = { 0 };
    HKEY hProgIdKey = nullptr;

    hr = StringCbPrintf(szProgIdRegKey, sizeof(szProgIdRegKey), TEXT("%s\\CLSID"), g_szProgId);
    if (FAILED(hr))
        goto done;

    // HKCR\ProgId\CLSID
    lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT, szProgIdRegKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE | KEY_CREATE_SUB_KEY, NULL, &hProgIdKey, NULL);
    OnErrorJump(hr, lRet, done);

    // HKCR\ProgId\CLSID @=ClassGUID
    lRet = RegSetValueEx(hProgIdKey, NULL, 0, REG_SZ, (PBYTE)szClassId, static_cast<DWORD>(wcslen(szClassId) * sizeof(wchar_t)));
    OnErrorJump(hr, lRet, done);

done:
    if (hProgIdKey)
        RegCloseKey(hProgIdKey);

    return hr;
}

STDAPI AddClassIdEntry(LPOLESTR szClassId, LPOLESTR szTypelibId, LPTSTR szFileName)
{
    HRESULT hr = S_OK;
    LONG lRet = ERROR_SUCCESS;
    TCHAR szClassIdRegKey[60] = { 0 };
    HKEY hClsidKey = nullptr, hInProcKey = nullptr, hTypelibKey = nullptr, hVersionKey = nullptr;

    hr = StringCbPrintf(szClassIdRegKey, sizeof(szClassIdRegKey), TEXT("CLSID\\%s"), szClassId);
    if (FAILED(hr))
        goto done;

    // HKCR\CLSID\ClassGUID
    lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT, szClassIdRegKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE | KEY_CREATE_SUB_KEY, NULL, &hClsidKey, NULL);
    OnErrorJump(hr, lRet, done);

    // HKCR\CLSID\ClassGUID @=ClassDescription
    lRet = RegSetValueEx(hClsidKey, NULL, 0, REG_SZ, (PBYTE)g_szClassDescription, sizeof(g_szClassDescription));
    OnErrorJump(hr, lRet, done);

    // HKCR\CLSID\ClassGUID\InprocServer32
    lRet = RegCreateKeyEx(hClsidKey, g_szInProcServer, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hInProcKey, NULL);
    OnErrorJump(hr, lRet, done);

    // HKCR\CLSID\ClassGUID\InprocServer32 @=PathToDll
    lRet = RegSetValueEx(hInProcKey, NULL, 0, REG_SZ, (PBYTE)szFileName, static_cast<DWORD>(wcslen(szFileName) * sizeof(wchar_t)));
    OnErrorJump(hr, lRet, done);

    // HKCR\CLSID\ClassGUID\InprocServer32 ThreadingModel=Both
    lRet = RegSetValueEx(hInProcKey, g_szThreadingModel, 0, REG_SZ, (PBYTE)g_szBoth, sizeof(g_szBoth));
    OnErrorJump(hr, lRet, done);

    // HKCR\CLSID\ClassGUID\TypeLib
    lRet = RegCreateKeyEx(hClsidKey, g_szTypeLib, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hTypelibKey, NULL);
    OnErrorJump(hr, lRet, done);

    // HKCR\CLSID\ClassGUID\TypeLib @=TypelibGUID
    lRet = RegSetValueEx(hTypelibKey, NULL, 0, REG_SZ, (PBYTE)szTypelibId, static_cast<DWORD>(wcslen(szTypelibId) * sizeof(wchar_t)));
    OnErrorJump(hr, lRet, done);

    // HKCR\CLSID\ClassGUID\Version
    lRet = RegCreateKeyEx(hClsidKey, g_szVersion, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hVersionKey, NULL);
    OnErrorJump(hr, lRet, done);

    // HKCR\CLSID\ClassGUID\Version @=1.0
    lRet = RegSetValueEx(hVersionKey, NULL, 0, REG_SZ, (PBYTE)g_szVersionValue, sizeof(g_szVersionValue));
    OnErrorJump(hr, lRet, done);

done:
    if (hInProcKey)
        RegCloseKey(hInProcKey);

    if (hClsidKey)
        RegCloseKey(hClsidKey);

    if (hTypelibKey)
        RegCloseKey(hTypelibKey);

    if (hVersionKey)
        RegCloseKey(hVersionKey);

    return hr;
}

STDAPI AddTypeLibIdEntry(LPOLESTR szTypelibId)
{
    HRESULT hr = S_OK;
    LONG lRet = ERROR_SUCCESS;
    TCHAR szTypelibIdRegKey[60] = { 0 };
    HKEY hTypelibIdKey = nullptr;

    hr = StringCbPrintf(szTypelibIdRegKey, sizeof(szTypelibIdRegKey), TEXT("TypeLib\\%s"), szTypelibId);
    if (FAILED(hr))
        goto done;

    // HKCR\TypeLib\TypeLibGUID
    lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT, szTypelibIdRegKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE | KEY_CREATE_SUB_KEY, NULL, &hTypelibIdKey, NULL);
    OnErrorJump(hr, lRet, done);

done:
    if (hTypelibIdKey)
        RegCloseKey(hTypelibIdKey);

    return hr;
}

STDAPI DllRegisterServer(void)
{
    HRESULT hr = S_OK;
    LONG lRet = ERROR_SUCCESS;
    LPOLESTR szClassId = nullptr, szTypelibId = nullptr, szInterfaceId = nullptr;

    TCHAR szInterfaceIdRegKey[60] = { 0 };
    TCHAR szFileName[MAX_PATH] = { 0 };

    LOG("Entering");

    CoInitialize(NULL);

    lRet = GetModuleFileName(g_hModule, szFileName, sizeof(szFileName));
    if (lRet == 0) {
        hr = HRESULT_FROM_WIN32(GetLastError());
        goto done;
    }

    hr = StringFromCLSID(CLSID_AddObject, &szClassId);
    if (FAILED(hr))
        goto done;

    hr = StringFromCLSID(LIBID_SuperFastMathLib, &szTypelibId);
    if (FAILED(hr))
        goto done;

    hr = StringFromCLSID(IID_IAdd, &szInterfaceId);
    if (FAILED(hr))
        goto done;

    //
    // ProgID
    //
    hr = AddProgIdEntry(szClassId);
    if (FAILED(hr))
        goto done;

    //
    // CLSID
    //
    hr = AddClassIdEntry(szClassId, szTypelibId, szFileName);
    if (FAILED(hr))
        goto done;

    //
    // TypeLib ID
    //
    hr = AddTypeLibIdEntry(szTypelibId);
    if (FAILED(hr))
        goto done;

done:
    if (szClassId)
        CoTaskMemFree(szClassId);

    if (szTypelibId)
        CoTaskMemFree(szTypelibId);

    if (szInterfaceId)
        CoTaskMemFree(szInterfaceId);

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
