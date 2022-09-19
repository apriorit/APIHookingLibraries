#include "pch.h"
#include "Hooker.h"

BOOL APIENTRY DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved)
{
    if (DetourIsHelperProcess())
    {
        return TRUE;
    }
    
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        HookFunction();
        break;
    
    case DLL_PROCESS_DETACH:
        UnhookFunction();
        break;
    }

    return TRUE;
}
