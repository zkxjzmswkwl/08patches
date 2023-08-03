#include "pch.h"
#include "Patches.h"
#include "Prep.h"

void Main()
{
    auto prep = new Prep();
    prep->SuspendApplicationThreads();

    // Patch here
    Patches::DebuggerPresent();

    for ( ;; )
    {
        if (GetAsyncKeyState(VK_F1) & 1)
        {
            break;
        }
        Sleep(10);
    }

    prep->ResumeApplicationThreads();
    delete prep;
}
//---------------------------------------------------------------------------
uintptr DadeMurphy(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    Main();

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
}
//---------------------------------------------------------------------------
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)DadeMurphy, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

