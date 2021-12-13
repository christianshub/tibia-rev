#include "injection.h"

HANDLE InjectDLL::TrySeDebugPriv(DWORD processID) {
    std::cout << " === TrySeDebugPriv() ===\n";
    HANDLE Token;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &Token) == FALSE)
    {
        std::cout << "OpenProcessToken error" << std::endl;
    }

    LUID Luid;

    if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &Luid) == FALSE)
    {
        std::cout << "LookupPrivilegeValueW error" << std::endl;
    }

    TOKEN_PRIVILEGES NewState;
    NewState.PrivilegeCount = 1;
    NewState.Privileges[0].Luid = Luid;
    NewState.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (AdjustTokenPrivileges(Token, FALSE, &NewState, sizeof(NewState), NULL, NULL) == 0
        || GetLastError() == ERROR_NOT_ALL_ASSIGNED)
    {
        std::cout << "AdjustTokenPrivileges error" << std::endl;
    }

    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);

    if (handle == NULL)
    {
        std::cout << "OpenProcess error" << std::endl;
    }

    // By now, we no longer need SeDebugPrivilege. We can drop/disable this privilege or restore
    // privileges to their previous state by re-adjusting the token.

    CloseHandle(Token);

    std::cout << " Returning handle: " << handle;
    return handle;
}

bool InjectDLL::Inject(const char* fullDllpath, PCSTR name) {
    std::cout << " === Inject() ===\n";
    std::cout << "fullDllPath: " << fullDllpath;
    std::cout << "name: " << name;

    DWORD pid = GetPID(name);

    std::cout << "PID: " << pid;

    // Step 0: Check if file exists
    struct stat buffer;
    if (stat(fullDllpath, &buffer) != 0)
    {
        std::cout << "\nCould not find DLL file.\n";
        return false;
    }

    // Step 1: Get the address of kernel32.dll's exported function: LoadLibraryA
    LPVOID loadLibrary = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");

    if (!loadLibrary)
    {
        std::cout << "\nCould not get address kernel32.dll's LoadLibraryA.\n";
        return false;
    }

    // Step 2: Get process handle from the target process
    HANDLE pHandle;

    pHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (!pHandle)
    {
        std::cout << "Could not get handle using OpenProcess, trying to set debug privilegde" << std::endl;
        pHandle = TrySeDebugPriv(pid);

        if (pHandle == NULL)
        {
            std::cout << "\nDid NOT obtain handle.\n" << std::endl;
            return false;
        }

        std::cout << "Did obtain handle: " << pHandle << std::endl;
    }

    // Step 3: Allocate space to write the dll location
    LPVOID dllParamAddr = VirtualAllocEx(pHandle, 0, strlen(fullDllpath), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!dllParamAddr)
    {
        CloseHandle(pHandle);
        std::cout << "\nCould not write DLL location using 'VirtualAllocEx'" << std::endl;
        return false;

        return 1;
    }

    // Step 4: Write the dll location to the space we allocated in step 3
    BOOL writeMem = WriteProcessMemory(pHandle, dllParamAddr, fullDllpath, strlen(fullDllpath), NULL);
    if (!writeMem)
    {
        CloseHandle(pHandle);
        std::cout << "\nCould not write to process memory using 'WPM'" << std::endl;
        return false;
    }

    // Step 5: Load the dll using LoadLibraryA from step 1
    HANDLE dllThreadHandle = CreateRemoteThread(pHandle, 0, 0, (LPTHREAD_START_ROUTINE)loadLibrary, dllParamAddr, 0, 0);
    if (!dllThreadHandle)
    {
        CloseHandle(pHandle);
        std::cout << "\nCould not create a remote thread using 'CreateRemoteThread'" << std::endl;
        return false;
    }

    // Step 6: Close handles
    CloseHandle(dllThreadHandle);
    CloseHandle(pHandle);
    return true;
}