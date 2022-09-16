#include "pch.h"
#include "Hooker.h"

extern "C" BOOL APIENTRY DllMain(DWORD  ul_reason_for_call)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        HRESULT hookErr = HookFunction();
        if (SUCCEEDED(hookErr))
        {
            OutputDebugStringW(L"DeviarePlugin.dll:DllMain - DLL_PROCESS_ATTACH: Successfully set hook");
        }
        else
        {
            OutputDebugStringW(L"DeviarePlugin.dll:DllMain - DLL_PROCESS_ATTACH: Error: Failed to set hook.");
        }
        break;
    }
    case DLL_PROCESS_DETACH:
        HRESULT unhookError = UnhookFunction();
        if (SUCCEEDED(unhookError))
        {
            OutputDebugStringW(L"DeviarePlugin.dll:DllMain - DLL_PROCESS_DETACH: Successfully unhook");
        }
        else
        {
            OutputDebugStringW(L"DeviarePlugin.dll:DllMain - DLL_PROCESS_DETACH: Error: Failed to unhook.");
        }
        break;
    }

    return TRUE;
}
