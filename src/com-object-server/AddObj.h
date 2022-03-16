//
// AddObj.h
// Contains the C++ class declaration for the IAdd interface
//
#pragma once
#include "IAdd_h.h" // Contains the C++ style interface declarations.

extern long g_nComObjectsInUse;

class CAddObj: public IAdd
{
public:
    CAddObj();

    // IUnknown interface
    HRESULT __stdcall QueryInterface(REFIID riid, void** ppObj);
    ULONG   __stdcall AddRef();
    ULONG   __stdcall Release();

    // IAdd interface
    HRESULT __stdcall SetFirstNumber(long nX1);
    HRESULT __stdcall SetSecondNumber(long nX2);
    HRESULT __stdcall PerformAddition(long* pSum);

private:
    long m_nX1, m_nX2; // operands for addition
    long m_nRefCount;  // for managing the reference count
};