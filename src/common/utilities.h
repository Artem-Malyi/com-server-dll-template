//
// utilities.h
// Contains declarations for the helper functions
//
#pragma once
#include "..\common\pch.h"

#define GUID_STRING_LENGTH (37)

BOOL GuidToWideString(_In_ REFIID riid, _Inout_ PWSTR wsGuidBuffer, _In_ SIZE_T wsGuidBufferSize);
BOOL GetInterfaceName(_In_ REFIID riid, _Inout_ PWSTR wsGuidBuffer, _In_ SIZE_T wsGuidBufferSize);