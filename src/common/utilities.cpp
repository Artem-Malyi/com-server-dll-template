//
// utilities.cpp
// Contains different helper functions
//
#include "..\common\pch.h"
#include "utilities.h"

#define DEBUG_LOGGER_ENABLED
#define FILE_LOGGER_ENABLED
#define LOG_PREFIX "[ADDOBJ-UTILZZZ]"
#include "logger.h"

//
// Helper functions
//
BOOL GuidToWideString(_In_ REFIID riid, _Inout_ PWSTR wsGuidBuffer, _In_ SIZE_T wsGuidBufferSize)
{
    if (!wsGuidBuffer || wsGuidBufferSize <= 0)
        return FALSE;

    HRESULT hr = StringCchPrintfW(
        wsGuidBuffer, wsGuidBufferSize, L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        riid.Data1,
        riid.Data2,
        riid.Data3,
        riid.Data4[0],
        riid.Data4[1],
        riid.Data4[2],
        riid.Data4[3],
        riid.Data4[4],
        riid.Data4[5],
        riid.Data4[6],
        riid.Data4[7]
    );
    if (FAILED(hr))
        return FALSE;

    return TRUE;
}

BOOL GetInterfaceName(_In_ REFIID riid, _Inout_ PWSTR wsGuidBuffer, _In_ SIZE_T wsGuidBufferSize)
{
    HRESULT hr = S_OK;
    LSTATUS lRet = ERROR_SUCCESS;
    LPOLESTR pwszInterfaceIDString = NULL;
    LPOLESTR lpszGuid = NULL;
    TCHAR szRegKey[50] = { 0 };
    DWORD dwType = 0;
    DWORD dwDataSize = static_cast<DWORD>(wsGuidBufferSize);

    if (!wsGuidBuffer || wsGuidBufferSize <= 0)
        goto done;

    // Assuming the caller has already initialized the COM library
    // CoInitialize(NULL);

    hr = StringFromIID(riid, &lpszGuid);
    if (FAILED(hr))
        goto done;

    hr = StringCbPrintf(szRegKey, sizeof(szRegKey), TEXT("Interface\\%s"), lpszGuid);
    if (FAILED(hr))
        goto done;

    lRet = RegGetValue(
        HKEY_CLASSES_ROOT,
        szRegKey,
        NULL, // This would obtain the default value.
        RRF_RT_REG_SZ,
        &dwType,
        wsGuidBuffer,
        &dwDataSize
    );

done:
    if (lpszGuid != NULL)
        CoTaskMemFree(lpszGuid);

    // CoUninitialize();

    LOG("Exiting with 0x%08x", lRet);

    return (lRet == ERROR_SUCCESS);
}

