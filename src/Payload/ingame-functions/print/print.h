#pragma once
#include "iostream"
#include "../../globals.h"
#include "../trampoline.h"

namespace print
{
	namespace
	{
		typedef void(__fastcall* print)(const char* ecx);
		print originalAddress = (print)(globals::moduleBase + 0x214540);
	}

	void Print(const char* text)
	{
		std::cout << "print::Print(" << text << ")" << "\n";
		originalAddress(text);
	}

	void GetAddress()
	{
		std::cout << "print::GetAddress(): " << std::hex << originalAddress << "\n";
	}

	void __fastcall Hook(const char* ecx)
	{
		std::cout << "printHook::hook, info: " << ecx << "\n";
	}

	void InitHook()
	{
		std::cout << "printHook::init()\n";
		Trampoline((PBYTE)originalAddress, (PBYTE)Hook, 5);
	}
}