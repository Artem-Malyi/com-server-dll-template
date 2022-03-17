// dllmain.cpp : Defines the entry point for the DLL application.
#include "..\common\pch.h"

#define DEBUG_LOGGER_ENABLED
#define FILE_LOGGER_ENABLED
#define LOG_PREFIX "[ADDOBJ-DLLMAIN]"
#include "logger.h"

long g_nComObjectsInUse = 0;
HMODULE g_hModule = nullptr;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    LOG("Entering, hModule: 0x%p, ul_reason_for_call: 0x%08x, lpReseved: 0x%p", hModule, ul_reason_for_call, lpReserved);
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        LOG("DLL_PROCESS_ATTACH: g_nComObjectsInUse: 0x%08x, g_hModule: 0x%p", g_nComObjectsInUse, g_hModule);
        break;
    case DLL_THREAD_ATTACH:
        LOG("DLL_THREAD_ATTACH: g_nComObjectsInUse: 0x%08x, g_hModule: 0x%p", g_nComObjectsInUse, g_hModule);
        break;
    case DLL_THREAD_DETACH:
        LOG("DLL_THREAD_DETACH: g_nComObjectsInUse: 0x%08x, g_hModule: 0x%p", g_nComObjectsInUse, g_hModule);
        break;
    case DLL_PROCESS_DETACH:
        LOG("DLL_PROCESS_DETACH: g_nComObjectsInUse: 0x%08x, g_hModule: 0x%p", g_nComObjectsInUse, g_hModule);
        break;
    }
    return TRUE;
}

