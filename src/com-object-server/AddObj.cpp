//
// AddObj.cpp
// Contains the implementations of IUnknown and IAdd interfaces
//
#include "..\common\pch.h"
#include "objbase.h"
#include "AddObj.h"
#include "IAdd_i.c" // Contains the interface IIDs
#include "strsafe.h"

#define DEBUG_LOGGER_ENABLED
#define FILE_LOGGER_ENABLED
#define LOG_PREFIX "[COM-ADDOBJ]"
#include "logger.h"

#define GUID_STRING_LENGTH (37)
BOOL GuidToWideString(_In_ LPGUID lpGuid, _Inout_ PWSTR wsGuidBuffer, _In_ SIZE_T wsGuidBufferSize);

//
// IAdd interface implementation
//
HRESULT __stdcall CAddObj::QueryInterface(REFIID riid, void** ppObj)
{
    LOG("Entering");

    WCHAR wsIID[GUID_STRING_LENGTH] = { 0 };
    BOOL bRes = GuidToWideString((LPGUID)&riid, wsIID, GUID_STRING_LENGTH);
    if (!bRes || !ppObj || !*ppObj)
        return E_INVALIDARG;

    LOG("IID: %s, ppObj: 0x%p, pObj: 0x%p", wsIID, ppObj, *ppObj);

    if (riid == IID_IUnknown) {
        *ppObj = static_cast<void*>(this);
        AddRef();
        LOG("Query for IUnknown, refCount: %d", m_nRefCount);
        return S_OK;
    }

    if (riid == IID_IAdd) {
        *ppObj = static_cast<void*>(this);
        AddRef();
        LOG("Query for IAdd, refCount: %d", m_nRefCount);
        return S_OK;
    }

    LOG("Not supported interface: %s", wsIID);
    *ppObj = nullptr;
    return E_NOINTERFACE;
}

ULONG __stdcall CAddObj::AddRef() {
    LOG("On instance 0x%p", this);
    return InterlockedIncrement(&m_nRefCount);
}

ULONG __stdcall CAddObj::Release() {
    LOG("On instance 0x%p", this);
    long nRefCount = 0;
    nRefCount = InterlockedDecrement(&m_nRefCount);
    if (0 == nRefCount) {
        LOG("Cleanup instance 0x%p", this);
        delete this;
    }
    return nRefCount;
}

//
// IAdd interface implementation
//
HRESULT __stdcall CAddObj::SetFirstNumber(long nX1)
{
    m_nX1 = nX1;
    LOG("X1: %d", m_nX1);
    return S_OK;
}

HRESULT __stdcall CAddObj::SetSecondNumber(long nX2)
{
    m_nX2 = nX2;
    LOG("X2: %d", m_nX2);
    return S_OK;
}

HRESULT __stdcall CAddObj::PerformAddition(long* pSum)
{
    if (!pSum) {
        LOG("Invalid argument. Exiting");
        return E_INVALIDARG;
    }

    *pSum = m_nX1 + m_nX2;
    LOG("SuperFast addition algorithm produced the result: %d", *pSum);

    return S_OK;
}

//
// Helper functions
//
BOOL GuidToWideString(_In_ LPGUID lpGuid, _Inout_ PWSTR wsGuidBuffer, _In_ SIZE_T wsGuidBufferSize)
{
    if (!lpGuid || !wsGuidBuffer || wsGuidBufferSize <= 0)
        return FALSE;

    HRESULT hr = StringCchPrintfW(
        wsGuidBuffer, wsGuidBufferSize, L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        lpGuid->Data1,
        lpGuid->Data2,
        lpGuid->Data3,
        lpGuid->Data4[0],
        lpGuid->Data4[1],
        lpGuid->Data4[2],
        lpGuid->Data4[3],
        lpGuid->Data4[4],
        lpGuid->Data4[5],
        lpGuid->Data4[6],
        lpGuid->Data4[7]
    );
    if (FAILED(hr))
        return FALSE;

    return TRUE;
}