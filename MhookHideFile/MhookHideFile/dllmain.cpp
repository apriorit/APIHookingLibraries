#include "pch.h"
#include "Hooker.h"

BOOL APIENTRY DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved)
{
    BOOL result = FALSE;
    
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        result = HookFunction();
        if (result)
        {
            OutputDebugStringW(L"MhookHideFile.dll:DllMain - DLL_PROCESS_ATTACH: Successfully set hook");
        }
        else
        {
            OutputDebugStringW(L"MhookHideFile.dll:DllMain - DLL_PROCESS_ATTACH: Error: Failed to set hook.");
        }
        break;
    case DLL_PROCESS_DETACH:
        result = UnhookFunction();
        if (result)
        {
            OutputDebugStringW(L"MhookHideFile.dll:DllMain - DLL_PROCESS_DETACH: Successfully unhook");
        }
        else
        {
            OutputDebugStringW(L"MhookHideFile.dll:DllMain - DLL_PROCESS_DETACH: Error: Failed to unhook.");
        }
        break;
    }

    return TRUE;
}
