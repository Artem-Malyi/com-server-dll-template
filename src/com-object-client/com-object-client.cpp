// com-object-client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#define DEBUG_LOGGER_ENABLED
#define FILE_LOGGER_ENABLED
#define LOG_PREFIX "[ADDOBJ-CLIENT]"
#include "logger.h"

#include "IAdd_h.h"
#include "IAdd_i.c"

//
// Here we do a #import on the DLL, you can also do a #import on the .TLB
// The #import directive generates two files (.tlh/.tli) in the output folders.
//
//#import "IAdd.tlb"

int main()
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

