#pragma once

#include <iostream>
#include <windows.h>
#include <sstream>
#include <iomanip>
#include <string>
#include <clocale>
#include <locale>
#include <vector>

namespace convert
{
std::string CHAR_ARRAY_TO_HEXSTR(char* data, int len);
std::string COMPLETE_HEX_STRUCT(std::string incompleteHexStruct);
std::string ULINT_TO_HEXSTR(unsigned long uint);
std::string INT_TO_HEXSTRING(int input);
std::string PCWSTR_TO_STRING(PCWSTR string);
std::string PBYTE_TO_HEXSTR(PBYTE data, int len);
std::string WSTRING_TO_STRING(std::wstring internal);

char* PWCHAR_TO_PCHAR(wchar_t* string);
}