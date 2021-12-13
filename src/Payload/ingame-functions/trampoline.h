#pragma once

#include <iostream>
#include <windows.h>

namespace hook
{
	class Hook {
		void* tToHook;
		char* oldOpcodes;
		int tLen;
	public:
		Hook(void* toHook, void* ourFunct, int len) : tToHook(toHook), oldOpcodes(nullptr), tLen(len) {
			if (len < 5) {
				return;
			}

			DWORD curProtection;
			VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

			oldOpcodes = (char*)malloc(len);
			if (oldOpcodes != nullptr) {
				for (int i = 0; i < len; ++i) {
					oldOpcodes[i] = ((char*)toHook)[i];
				}
			}

			memset(toHook, 0x90, len);

			DWORD relativeAddress = ((DWORD)ourFunct - (DWORD)toHook) - 5;

			*(BYTE*)toHook = 0xE9;
			*(DWORD*)((DWORD)toHook + 1) = relativeAddress;

			VirtualProtect(toHook, len, curProtection, &curProtection);
		}

		~Hook() {
			if (oldOpcodes != nullptr) {
				DWORD curProtection;
				VirtualProtect(tToHook, tLen, PAGE_EXECUTE_READWRITE, &curProtection);
				for (int i = 0; i < tLen; ++i) {
					((char*)tToHook)[i] = oldOpcodes[i];
				}
				VirtualProtect(tToHook, tLen, curProtection, &curProtection);
				free(oldOpcodes);
			}
		}
	};
}


/// <summary>
/// Move execution flow from original function -> hook function -> code cave -> original function.
/// Avoids hook recursion.
/// </summary>
/// <param name="source">original function</param>
/// <param name="destination">hook function</param>
/// <param name="byteLen">amount of bytes to steal</param>
/// <returns>jump back to original function</returns>
BYTE* Trampoline(PBYTE source, PBYTE destination, unsigned int byteLen)
{
	if (byteLen < 5) {
		std::cout << "[-] Trampoline Hook Activation: FAILED\n";
		std::cout << "[-] REASON: Need atleast 5 bytes to call or jump address\n";
		return 0;
	}

	// ***************************
	//    Trampoline/Code cave 
	// ***************************

	// Allocate a page with RWX access for our code cave. 
	BYTE* trampoline = (BYTE*)VirtualAlloc(0, byteLen, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	// Save bytes from source to code cave.
	memcpy_s(trampoline, byteLen, source, byteLen);

	// Calculate RVA from source to code cave, jump over the 5 first bytes of source
	uintptr_t trampolineRVA = source - trampoline - byteLen;

	// Add a jmp to beginning of code cave
	*(trampoline + byteLen) = 0xE9;

	// After the jmp, add the RVA. Se we jmp to source
	*(uintptr_t*)((uintptr_t)trampoline + byteLen + 1) = trampolineRVA;

	// **************
	//     Detour  
	// **************

	// Give RWX access to original function's bytes
	DWORD currentProtection;
	VirtualProtect(source, byteLen, PAGE_EXECUTE_READWRITE, &currentProtection);

	// Calculate RVA from hook function (dest) to original function (src) - 5 first bytes 
	uintptr_t relativeAddress = destination - source - byteLen;

	// At source, fill with nops
	memset(source, 0x90, byteLen);

	// At source, replace the first byte with a jmp
	*source = 0xE9;

	// Add RVA after jump. So we jump from original function to hook function 
	*(uintptr_t*)(source + 1) = relativeAddress;

	// Restore protection access to original function's bytes
	VirtualProtect(source, byteLen, currentProtection, &currentProtection);

	return trampoline;
}