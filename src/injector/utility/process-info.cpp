#include "process-info.h"

DWORD GetPID(PCSTR name) {
    std::cout << " === GetPID() ===\n" << std::endl;
    std::cout << "Name of process we are looking for: " << name << "\n" << std::endl;
    DWORD pid = 0;

    // Create toolhelp snapshot.
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 process;
    ZeroMemory(&process, sizeof(process));
    process.dwSize = sizeof(process);

    // Walkthrough all processes.
    if (Process32First(snapshot, &process))
    {
        do
        {
            // Compare process.szExeFile based on format of name, i.e., trim file path
            // trim .exe if necessary, etc.
            // TODO: Write down the needed filenames

            //std::cout << "Process: " << std::string(process.szExeFile);

            if (std::string(process.szExeFile) == std::string(name))
            {
                std::cout << "Match! process: " << std::string(process.szExeFile) << "\n" << std::endl;
                pid = process.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &process));
    }

    CloseHandle(snapshot);

    if (pid != 0)
    {
        std::cout << "Returning PID: " << pid << "\n" << std::endl;
        return pid;
    }

    // Not found
    std::cout << "Did not find pid: " << pid << "\n" << std::endl;

    return 0;
}