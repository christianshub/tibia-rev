#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include "utility/injection.h"
#include "config/config.h"
#include "config/parser.h"

using namespace std;

int main() {

    std::cout << " === main() ===\n";

    //Create folder and ini file with content (if it doesn't exist)
    string configPath = VerifyINI("Injector", "config.ini", { "[Config]", "DLLs=", "msDelay=", "TargetProcess=", });
    std::cout << "configPath: " + configPath;

    // Read .ini file 
    string DLLs = ReadKey("Config", "DLLs", configPath);
    string Delay = ReadKey("Config", "msDelay", configPath);
    string TargetProcess = ReadKey("Config", "TargetProcess", configPath);

    std::cout << "DLLs:          " << DLLs << std::endl;
    std::cout << "Delay (ms):    " << Delay << " (between each injection)" << std::endl;
    std::cout << "TargetProcess: " << TargetProcess << std::endl;

    unsigned int delay = ParseDelay(Delay);

    // Parse config
    vector<string> dlls = ParseFileNames(DLLs);
    vector<string> targetProcesses = ParseFileNames(TargetProcess);

    // Catch if no DLL's are given in the config.ini file:
    if (dlls.size() == 0 || targetProcesses.size() == 0)
    {
        std::cout << "No DLL or TargetProcesses found" << std::endl;;
        return 1;
    }

    std::cout << "Looping over amount of targetProcesses " << targetProcesses.size() << " times" << std::endl;
    for (size_t i = 0; i < targetProcesses.size(); i++) {
        std::cout << "Looping over amount of DLL's " << dlls.size() << " times" << std::endl;
        for (size_t j = 0; j < dlls.size(); j++) {
            std::cout << "Dll " << j << " name: " << dlls[j].c_str();
            Sleep(delay);

            InjectDLL injector;
            if (injector.Inject(dlls[j].c_str(), targetProcesses[i].c_str())) {
                std::cout << "[+] SUCCESS injecting: " << dlls[j] << endl;
                Sleep(10000);
            }
            else {
                std::cout << "[-] FAILED injecting " << dlls[j] << endl;
            }
        }
    }

    Sleep(5000);

    return 0;
}
