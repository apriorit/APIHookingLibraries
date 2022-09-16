#include <iostream>
#include <string>
#include <Windows.h>
#include <Psapi.h> // GetModuleFileNameExW

//Allow calls to the Deviare library-------------------------
#include <NktHookLib.h>
#if _WIN64
#pragma comment(lib, "NktHookLib64")
#else
#pragma comment(lib, "NktHookLib")
#endif
//-----------------------------------------------------------

int __CRTDECL wmain(__in int argc, __in wchar_t* argv[])
{
    if (argc < 2)
    {
        std::wcout << L"Use: InjectDLL.exe Process-ID" << std::endl;
        return 1;
    }

    // Trying to parse the Process ID
    const DWORD dwPid = _wtoi(argv[1]);
    if (dwPid == 0 || dwPid == INT_MAX || dwPid == INT_MIN)
    {
        char errmsg[80];
        std::wcout << L"Error: Unable to parse \"Process-ID\" : " << strerror_s(errmsg, 80, errno) << std::endl;
        std::wcout << L"Use: InjectDLL.exe Process-ID, where Process-ID is a digit" << std::endl;
        return 1;
    }

    if (dwPid == GetCurrentProcessId())
    {
        std::wcout << L"Error: Cannot inject a dll into Process-ID." << std::endl;
        return 1;
    }

    // Open specified process
    const HANDLE process = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, dwPid);
    if (process == NULL)
    {
        std::wcout << L"Error: Cannot open provided a process for terminating - GetLastError: " << GetLastError() << std::endl;
        return 1;
    }

    // Gets full path of the specified process
    TCHAR exeNameW[_MAX_PATH];
    if (!GetModuleFileNameExW(process, NULL, exeNameW, _MAX_PATH))
    {
        std::cout << "Error: Cannot get a process name or PID does not exist - GetLastError: " << GetLastError() << std::endl;
        return 1;
    }

    // A restart of the specified process
    std::wcout << L"Warning: Specified process with PID: " << dwPid << " and with Name: " << exeNameW << " will be restarted." << std::endl;
    TerminateProcess(process, 1);
    CloseHandle(process);

    #if _WIN64
    const std::wstring dllToInjectNameW = L"DeviareHideFilePlugin_x64.dll";
    #else
    const std::wstring dllToInjectNameW = L"DeviareHideFilePlugin_Win32.dll";
    #endif

    // Create process with out custom DLL that hooks APIs
    STARTUPINFOW sSiW{};
    sSiW.cb = static_cast<DWORD>(sizeof(sSiW));
    PROCESS_INFORMATION sPi{};

    const DWORD dwOsErr = NktHookLibHelpers::CreateProcessWithDllW(exeNameW, NULL, NULL, NULL, FALSE,
        0, NULL, NULL, &sSiW, &sPi, dllToInjectNameW.c_str(), NULL, NULL);
    if (dwOsErr != ERROR_SUCCESS)
    {
        std::wcout << L"Error: " << dwOsErr << L" - Cannot launch process and inject dll." << std::endl;
        CloseHandle(sPi.hThread);
        CloseHandle(sPi.hProcess);
        return 1;
    }

    std::wcout << L"InjectDLL: Process #" << sPi.dwProcessId << L" successfully launched with dll injected!" << std::endl;
    
    CloseHandle(sPi.hThread);
    CloseHandle(sPi.hProcess);

    return 0;
}
