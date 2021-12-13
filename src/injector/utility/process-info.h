#pragma once
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <string>

/// <summary>
/// Retrieves process ID from an open application window.
/// </summary>
DWORD GetPID(PCSTR name);