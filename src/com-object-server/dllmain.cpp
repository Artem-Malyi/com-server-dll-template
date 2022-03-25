// dllmain.cpp : Defines the entry point for the DLL application.
#include "..\common\pch.h"

#define DEBUG_LOGGER_ENABLED
#define FILE_LOGGER_ENABLED
#define LOG_PREFIX "[ADDOBJ-DLLMAIN]"
#include "logger.h"

// Global COM server variables.
ULONG g_LockCount = 0;		// Represents the number of COM server locks.
ULONG g_ObjectCount = 0;	// Represents the number of living COM objects.

HMODULE g_hModule = nullptr;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    LOG("Entering, hModule: 0x%p, ul_reason_for_call: 0x%08x, lpReseved: 0x%p", hModule, ul_reason_for_call, lpReserved);
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        LOG("DLL_PROCESS_ATTACH: g_ObjectCount: 0x%08x, g_hModule: 0x%p", g_ObjectCount, g_hModule);
        break;
    case DLL_THREAD_ATTACH:
        LOG("DLL_THREAD_ATTACH: g_ObjectCount: 0x%08x, g_hModule: 0x%p", g_ObjectCount, g_hModule);
        break;
    case DLL_THREAD_DETACH:
        LOG("DLL_THREAD_DETACH: g_ObjectCount: 0x%08x, g_hModule: 0x%p", g_ObjectCount, g_hModule);
        break;
    case DLL_PROCESS_DETACH:
        LOG("DLL_PROCESS_DETACH: g_ObjectCount: 0x%08x, g_hModule: 0x%p", g_ObjectCount, g_hModule);
        break;
    }
    return TRUE;
}

