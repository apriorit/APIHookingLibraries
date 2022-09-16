#include "pch.h"
#include "Hooker.h"

BOOL APIENTRY DllMain(DWORD  ul_reason_for_call)
{
    if (DetourIsHelperProcess())
    {
        return TRUE;
    }
    
    switch (ul_reason_for_call)
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
