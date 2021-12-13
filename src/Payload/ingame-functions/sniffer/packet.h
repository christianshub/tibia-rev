#pragma once
#include "iostream"
#include "../../globals.h"
#include "../trampoline.h"

namespace packetSniffer
{
	namespace
	{
		typedef void(__stdcall* packet)(SOCKET s, const char* buf, int len, int flags);
		uintptr_t moduleBase = (uintptr_t)GetModuleHandle("ws2_32.dll");
		packet originalAddress = (packet)(moduleBase + 0x158A0);
		packet jumpbackAddress = nullptr;
	}

	void GetAddress()
	{
		std::cout << "packet::GetAddress (originalAddress): " << std::hex << originalAddress << "\n";
		std::cout << "packet::GetAddress (jumpbackAddress): " << std::hex << jumpbackAddress << "\n";
	}

	void __stdcall Hook(SOCKET s, const char* buf, int len, int flags)
	{
		if (buf != 0)
		{
			std::cout << "packet::Hook (len): " << len << "\n";
			std::cout << "packet::Hook (buf): " << globals::hexStr((char*)buf, len) << "\n";
		}

		jumpbackAddress(s, buf, len, flags);
	}

	void InitHook()
	{
		std::cout << "packetHook::Init()\n";
		jumpbackAddress = (packet) Trampoline((PBYTE)originalAddress, (PBYTE)Hook, 5);
	}
}

/*
Address  To       From     Siz Comment              Party
010FE7D4 006E30A1 75D958A0 18  ws2_32.75D958A0      User
010FE7EC 00626DB3 006E30A1 1AC tibia.sub_6E308C + 15  User
010FE998 0048D5E8 00626DB3 154 tibia.sub_626B10 + 2A3 User
010FEAEC 00610AAC 0048D5E8 1D4 tibia.0048D5E8       User
010FECC0 005D81A0 00610AAC 128 tibia.sub_6103F7 + 6B5 User
010FEDE8 005D81A0 005D81A0 128 tibia.sub_5D80F0 + B0  User
010FEF10 00516196 005D81A0 17C tibia.sub_5D80F0 + B0  User
010FF08C 005ED61F 00516196 120 tibia.00516196       User
010FF1AC 006DEE32 005ED61F BC  tibia.sub_5ED46E + 1B1 User
010FF268 006E00C3 006DEE32 20  tibia.006DEE32       User
010FF288 006DBA8C 006E00C3 70  tibia.sub_6E00A1 + 22  User
010FF2F8 006DC216 006DBA8C 20  tibia.006DBA8C       User
010FF318 769C339B 006DC216 2C  tibia.006DC216       System
010FF344 769BA33A 769C339B E4  user32.769C339B      System
010FF428 769B9A07 769BA33A 38  user32.769BA33A      System
010FF460 769B7BAB 769B9A07 18  user32.769B9A07      System
010FF478 709D6ACF 769B7BAB 44  user32.769B7BAB      System
010FF4BC 769C339B 709D6ACF 2C  opengl32.709D6ACF    System
010FF4E8 769BA33A 769C339B E4  user32.769C339B      System
010FF5CC 769B809A 769BA33A 74  user32.769BA33A      System
010FF640 769AA230 769B809A 8   user32.769B809A      System
010FF648 006E1E1D 769AA230 2C  user32.769AA230      User
010FF674 0063E5DD 006E1E1D 16C tibia.006E1E1D       User
010FF7E0 0084D980 0063E5DD 14  tibia.0063E5DD       User
010FF7F4 007FD31F 0084D980 4C  tibia.sub_84D911 + 6F  User
010FF840 75E0FA29 007FD31F 10  tibia.007FD31F       System
010FF850 773B7A9E 75E0FA29 5C  kernel32.75E0FA29    System
010FF8AC 773B7A6E 773B7A9E 10  ntdll.773B7A9E       System
010FF8BC 00000000 773B7A6E     ntdll.773B7A6E       
*/