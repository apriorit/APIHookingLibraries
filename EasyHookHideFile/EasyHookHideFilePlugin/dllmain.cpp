#include "pch.h"
#include "Hooker.h"

/*
    For remote native/unmanaged hooks, EasyHook is expecting to find an export named 'NativeInjectionEntryPoint' within your DLL. 
    EasyHook will call this method once the DLL has successfully been injected.
*/
extern "C" void __declspec(dllexport) WINAPI NativeInjectionEntryPoint(REMOTE_ENTRY_INFO * inRemoteInfo);

void WINAPI NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo)
{
    OutputDebugStringW(L"EasyHookHideFile.dll:NativeInjectionEntryPoint - Entered to the main function");

    // Install the hook
    HOOK_TRACE_INFO hHook = { NULL };
    NTSTATUS result = LhInstallHook(
        GetProcAddress(GetModuleHandle(TEXT("ntdll")), "NtQueryDirectoryFile"),
        (void*)HookNtQueryDirectoryFile,
        nullptr,
        &hHook);
    if (FAILED(result))
    {
        OutputDebugStringW(L"EasyHookHideFile.dll:NativeInjectionEntryPoint - Error: Failed to set hook.");
        return;
    }
    OutputDebugStringW(L"EasyHookHideFile.dll:NativeInjectionEntryPoint - Successfully set hook");

    // If the threadId in the ACL is set to 0, then internally EasyHook uses GetCurrentThreadId()
    // Activates hook for the thread
    ULONG ACLEntries[1] = { 0 };
    result = LhSetInclusiveACL(ACLEntries, 1, &hHook); 
    if (FAILED(result))
    {
        OutputDebugStringW(L"EasyHookHideFile.dll:NativeInjectionEntryPoint - Error: Failed to LhSetInclusiveACL.");
        return;
    }
    OutputDebugStringW(L"EasyHookHideFile.dll:NativeInjectionEntryPoint - Successfully activated hook");

    return;
}
