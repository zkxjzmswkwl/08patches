#include "pch.h"
#include "Patches.h"
#include "Prep.h"
#include "MinHook.h"
#include "Hooks.h"


void PlaceHooks()
{
    const auto ws2 = GetModuleHandle("ws2_32.dll");
    const auto ws2Send = GetProcAddress(ws2, "send");
    MH_CreateHook((LPVOID)ws2Send, &SendHook, (LPVOID*)&oSend);
}
//---------------------------------------------------------------------------
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

        if (GetAsyncKeyState(VK_F3) & 1)
        {
            prep->ResumeApplicationThreads();
        }
        Sleep(10);
    }

    prep->ResumeApplicationThreads();
    delete prep;
}
//---------------------------------------------------------------------------
FILE* Initialize()
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    return f;
}
//---------------------------------------------------------------------------
void Uninitialize(FILE* f, HMODULE hModule)
{
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
}
//---------------------------------------------------------------------------
uintptr DadeMurphy(HMODULE hModule)
{
    auto sout = Initialize();

    MH_Initialize();
    PlaceHooks();
    MH_EnableHook(MH_ALL_HOOKS);

    Main();

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    Uninitialize(sout, hModule);
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

