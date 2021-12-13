#pragma once 

#include "../../utility/convert.h"
#include "x64dbg-ntdll.h"

namespace sigscanner
{
    char* ScanBasic(char* pattern, char* mask, char* begin, intptr_t size);
    char* ScanInternal(const char* pattern, const char* mask, char* begin, intptr_t size);
    PEB* GetPEB();
    LDR_DATA_TABLE_ENTRY* GetLDREntry(std::string name);
    uintptr_t ScanModIn(const char* pattern, const char* mask, std::string modName);
}

