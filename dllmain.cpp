// dllmain.cpp : Defines the entry point of the DLL application.
#include "pch.h"

// DllMain is the entry point for a DLL. It is called by the system when processes and threads
// are initialized and terminated, or when LoadLibrary and FreeLibrary are called.
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    // The ul_reason_for_call parameter indicates why the DLL entry-point function is being called.
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // A process is loading the DLL.
        break;
    case DLL_THREAD_ATTACH:
        // A thread is being created in a process that has already loaded the DLL.
        break;
    case DLL_THREAD_DETACH:
        // A thread is exiting cleanly.
        break;
    case DLL_PROCESS_DETACH:
        // A process unloads the DLL.
        break;
    }
    // Return TRUE to indicate successful initialization.
    return TRUE;
}
