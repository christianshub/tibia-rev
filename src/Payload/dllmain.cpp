#pragma once
#include <windows.h>
#include <iostream>
#include "utility/keys.h"
#include "ingame-functions/print/print.h"
#include "ingame-functions/sniffer/packet.h"
#include "ingame-functions/follow/followPacket.h"
#include "ingame-functions/look/lookPacket.h"

DWORD WINAPI WindowThread(HMODULE hModule) {
    FILE* pFile = nullptr;

    AllocConsole();
    freopen_s(&pFile, "CONOUT$", "w", stdout);

    std::cout << "Press 'F1'    Print: Call\n";
    std::cout << "Press 'F2'    Print: Hook\n";
    std::cout << "Press 'F3'    Packet Sniffer: Hook\n";
    std::cout << "Press 'F4'    Look at cat\n";
    std::cout << "Press 'F5'    Follow cat\n";
   
    std::cout << "Press 'F12'   Detach\n";

    while (true) {

        if (IsKeyPressed_F1)
        {
            print::Print("test!");
        }

        if (IsKeyPressed_F2)
        {
            print::InitHook();
        }

        if (IsKeyPressed_F3)
        {
            packetSniffer::InitHook();
            packetSniffer::GetAddress();
        }

        if (IsKeyPressed_F4)
        {
            lookPacket::Send();
        }

        if (IsKeyPressed_F5)
        {
            followPacket::Send();
        }        
    
        if (IsKeyPressed_F12)
        {
            std::cout << "Detaching..." << "\n";
            break;
        }
    }

    fclose(pFile);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)WindowThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}