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
#import "com-object-server64d.dll"
#endif

int main()
{
    return testComServer();
}

#ifdef BUILD_CLIENT_WITH_TYPELIB
int testComServer()
{
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        return -1;

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
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        return -1;

    IAdd* pFastAddAlgorithm = nullptr;
    hr = CoCreateInstance(CLSID_AddObject, NULL, CLSCTX_INPROC_SERVER, IID_IAdd, (void**)&pFastAddAlgorithm);
    LOG("CoCreateInstance() returned 0x%08x", hr);
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