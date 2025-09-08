not in deep enough to pretend to be able to give coherent explanations just yet.
just a dump of things for switching between machines etc.

minhook to be replaced w cpp kronos port eventually.

```c++
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  const CHAR *LdrRegisterDllNotification_str; // rbx
  const CHAR *ntdll; // rax
  HMODULE ntdll_handle; // rax
  FARPROC LdrRegisterDllNotification; // rax
  char is_running_under_hypervisor; // di
  const CHAR *v20; // rax
  __int64 (__fastcall **v22)(); // [rsp+390h] [rbp+290h] BYREF
  char v23; // [rsp+798h] [rbp+698h]

  LoadStringA(hInstance, 0x67u, Caption, 100);
  // (carter): comp-gen'd function for each encrypted string.
  //           furry shit.
  LdrRegisterDllNotification_str = decrypt_LdrRegisterDllNotification_str();
  ntdll = decrypt_ntdll_str();
  ntdll_handle = GetModuleHandleA(ntdll);
  LdrRegisterDllNotification = GetProcAddress(ntdll_handle, LdrRegisterDllNotification_str);
  if ( LdrRegisterDllNotification )
    (LdrRegisterDllNotification)(0LL, DllCallback, 0LL, &unk_1418F8EB8);
  CoInitializeEx(0LL, 2u);
  _RAX = 0LL;
  __asm { cpuid }
  is_running_under_hypervisor = 0;
  // (carter): check for leaf 1
  if ( _RAX >= 1 )
  {
    // (carter): if supported, check for hypervisor.
    _RAX = 1LL;
    __asm { cpuid }
    is_running_under_hypervisor = _RCX & 1;
  }
  // (carter): &Caption[X] = 1 repeated ~50 times. Epic. Decomp artifact/something fucky.
  sub_1405A4670();
  register_tank_window(hInstance);
  if ( IsDebuggerPresent() )
  {
    // (carter): decrypts some string i don't give a fuck about
    v20 = sub_1405AB9D0();
    MessageBoxA(0LL, v20, Caption, 0x11010u);
    return -1;
  }
  else
  {
    if ( is_running_under_hypervisor )
    {
      LoadAcceleratorsA(hInstance, 0x6D);
      sub_1407CECB0(&v22);
      v23 = 0;
      v22 = off_141454AE8;
      // (carter): only does the coolshit if under an hv and not being debugged?
      //           does the same shit vmp does. reads ntdll from disk, identifies syscall idxs.
      //           and some other shit. if first fails, second attempt patches ntdll!DbgBreakPoint.
      //           noteably, gets an address for NtSetInformationThread.
      //           unlike vmp, u can't just set KUSER_SHARED_DATA winver to 6969 and fuck off.
      manully_map_ntdll_populate_syscall_idxs();
    }
    MessageBoxA(0LL, unsupported_cpu_detected_str, Caption, 69648u);
    return -1;
  }
}
  ```