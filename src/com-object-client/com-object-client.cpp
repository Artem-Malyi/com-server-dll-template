// com-object-client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define BUILD_CLIENT_WITH_TYPELIB
//#define BUILD_CLIENT_WITH_IDL_HEADERS

#include <iostream>
#include <objbase.h>

#define DEBUG_LOGGER_ENABLED
#define FILE_LOGGER_ENABLED
#define LOG_PREFIX "[ADDOBJ-CLIENT]"
#include "logger.h"

int testComServer();
bool errorDescription(DWORD dwErrorCode, PWSTR wszMsgBuff, DWORD wsMessageBufferSize);

#ifdef BUILD_CLIENT_WITH_IDL_HEADERS
//
// To make this work enable the Post-Build event in com-object-server project
//
#include "IAdd_h.h"
#include "IAdd_i.c"
#endif

#ifdef BUILD_CLIENT_WITH_TYPELIB
//
// Here we do a #import on the DLL, you can also do a #import on the .TLB
// The #import directive generates two files (.tlh/.tli) in the output folders.
//
//#import "IAdd.tlb"
#ifdef _WIN64
    #ifdef _DEBUG
        #import "com-object-server64d.dll"
    #else
        #import "com-object-server64.dll"
    #endif
#else
    #ifdef _DEBUG
        #import "com-object-server32d.dll"
    #else
        #import "com-object-server32.dll"
    #endif
#endif
#endif // BUILD_CLIENT_WITH_TYPELIB

int main()
{
    return testComServer();
}

#ifdef BUILD_CLIENT_WITH_TYPELIB
int testComServer()
{
    LOG("Entering");

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        return -1;

    WCHAR wsMessageBuffer[512] = { 0 };

    {
        SuperFastMathLib::IAddPtr pFastAddAlgorithm;
        //
        // IAddPtr is not the actual interface IAdd, but a template C++ class (_com_ptr_t)
        // that contains an embedded instance of the raw IAdd pointer.
        // While destructing, the destructor makes sure to invoke Release() on the internal
        // raw interface pointer. Further, the operator -> has been overloaded to direct all
        // method invocations to the internal raw interface pointer.
        //
        hr = pFastAddAlgorithm.CreateInstance("SuperFast.AddObj");
        errorDescription(hr, wsMessageBuffer, _countof(wsMessageBuffer));
        LOG("CoCreateInstance() returned 0x%08x: %ws", hr, wsMessageBuffer);
        if (FAILED(hr))
            return -2;

        long n1 = 100, n2 = 200;
        pFastAddAlgorithm->SetFirstNumber(n1); //"->" overloading in action
        pFastAddAlgorithm->SetSecondNumber(n2);

        long nSum = 0;
        nSum = pFastAddAlgorithm->PerformAddition();

        std::cout << "Output after adding " << n1 << " & " << n2 << " is " << nSum << "\n";
        LOG("SuperFast addition algorithm returned: %d + %d = %d", n1, n2, nSum);
    }

    CoUninitialize();

    return 0;
}
#endif

#ifdef BUILD_CLIENT_WITH_IDL_HEADERS
int testComServer()
{
    LOG("Entering");

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        return -1;

    WCHAR wsMessageBuffer[512] = { 0 };

    IAdd* pFastAddAlgorithm = nullptr;
    hr = CoCreateInstance(CLSID_AddObject, NULL, CLSCTX_INPROC_SERVER, IID_IAdd, (void**)&pFastAddAlgorithm);
    errorDescription(hr, wsMessageBuffer, _countof(wsMessageBuffer));
    LOG("CoCreateInstance() returned 0x%08x: %ws", hr, wsMessageBuffer);
    if (FAILED(hr)) {
        CoUninitialize();
        return -2;
    }

    long n1 = 100, n2 = 200;
    pFastAddAlgorithm->SetFirstNumber(n1); //"->" overloading in action
    pFastAddAlgorithm->SetSecondNumber(n2);

    long nSum = 0;
    hr = pFastAddAlgorithm->PerformAddition(&nSum);
    LOG("IAdd->PerformAddition() returned 0x%08x", hr);
    if (FAILED(hr)) {
        CoUninitialize();
        return -3;
    }

    std::cout << "Output after adding " << n1 << " & " << n2 << " is " << nSum << "\n";
    LOG("SuperFast addition algorithm returned: %d + %d = %d", n1, n2, nSum);

    //
    // Release the reference to the COM object when we're done
    //
    pFastAddAlgorithm->Release();

    CoUninitialize();

    return 0;
}
#endif

bool errorDescription(DWORD dwErrorCode, PWSTR wsMessageBuffer, DWORD wsMessageBufferSize)
{
    // Try to get the message from the system errors list.
    DWORD dwChars = FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwErrorCode,
        0,
        wsMessageBuffer,
        wsMessageBufferSize,
        NULL
    );
    if (0 == dwChars)
    {
        // The error code did not exist in the system errors list.
        // Try Ntdsbmsg.dll for the error code.
        HINSTANCE hInst = LoadLibrary(L"Ntdsbmsg.dll");
        if (!hInst)
            return false;

        // Try getting message text from ntdsbmsg.
        dwChars = FormatMessage(
            FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
            hInst,
            dwErrorCode,
            0,
            wsMessageBuffer,
            wsMessageBufferSize,
            NULL
        );

        FreeLibrary(hInst);
    }

    return (0 == dwChars);
}