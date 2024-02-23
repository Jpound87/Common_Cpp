// dllmain.cpp : Defines the entry point for the DLL application.
/*
The following tasks are safe to perform within DllMain:

Initialize static data structures and members at compile time.
Create and initialize synchronization objects.
Allocate memory and initialize dynamic data structures (avoiding the functions listed above.)
Set up thread local storage (TLS).
Open, read from, and write to files.
Call functions in Kernel32.dll (except the functions that are listed above).
Set global pointers to NULL, putting off the initialization of dynamic members. In Microsoft Windows Vista™, you can use the one-time initialization functions to ensure that a block of code is executed only once in a multithreaded environment.

*/
#include "pch.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}



