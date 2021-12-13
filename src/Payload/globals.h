#pragma once

#include <windows.h>

namespace globals
{
	uintptr_t moduleBase = (uintptr_t)GetModuleHandle("Tibia.exe");

    std::string hexStr(char* data, int len)
    {
        constexpr char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                                    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

        std::string s(len * 2, ' ');
        for (int i = 0; i < len; ++i) {
            s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
            s[2 * i + 1] = hexmap[data[i] & 0x0F];
        }
        return s;
    }
}