#pragma once

#include "process-info.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <sys/stat.h>
#include <iostream>

class InjectDLL {
    /// <summary>
    /// Sets debug privilegde
    /// </summary>
    /// <returns>a handle</returns>
    HANDLE TrySeDebugPriv(DWORD processID);
public:
    /// <summary>
    /// Injects a DLL into a target process
    /// </summary>
    bool Inject(const char* fullDllpath, PCSTR name);
};