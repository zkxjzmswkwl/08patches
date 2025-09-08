not in deep enough to pretend to be able to give coherent explanations just yet.
just a dump of things for switching between machines etc.

minhook to be replaced w cpp kronos port eventually.

- things
  -
    - https://jemalloc.net/
    - https://github.com/protocolbuffers/protobuf
    - some strings encrypted.
      - comp-time gen'd functions for decryption.
      - `ntdll.dll` is xor'd but for some reason shit like `NtSetInformationThread` isn't.
    - they manually map `ntdll.dll`. They read a copy from disk to yoink syscall idxs. Similarly to vmp.
      - Unlike vmp, you can't set `KUSER_SHARED_DATA`'s winver to 6969 and win.
    - Conditional patches on `ntdll!DbgBreakPoint`(?)
    - if anything like wow shit tons of crc checks.

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

// ---------------------------------------------------------------------------

// (carter): Iterates over an array of string pointers, if any match what's in data->Loaded.BaseDllName->Buffer, set g flags.
//           easyhook64.dll, fraps64.dll, ntdll.dll.
//           ntdll.dll being included in this seems like a bug.
void __fastcall DllCallback(ULONG notification_reason, _LDR_DLL_NOTIFICATION_DATA *notification_data)
{
  __int64 buffer_len; // rbx
  wchar_t *Buffer; // r8
  int v5; // r14d
  int buffer_mb_str; // eax
  int cbMultiByte; // r9d
  unsigned __int64 v8; // rdx
  __int64 v9; // rax
  unsigned __int64 v10; // rax
  void *v11; // rsp
  void *v12; // rsp
  __int64 v13; // rcx
  _BYTE **v14; // rbx
  _BYTE *v15; // r8
  CHAR *v16; // r9
  int v17; // eax
  char v18; // dl
  CHAR *i; // rax
  CHAR MultiByteStr[16]; // [rsp+40h] [rbp+0h] BYREF

  if ( notification_reason == 1 )
  {
    buffer_len = -1LL;
    Buffer = notification_data->Loaded.BaseDllName->Buffer;
    do
      ++buffer_len;
    while ( Buffer[buffer_len] );
    v5 = 0;
    buffer_mb_str = WideCharToMultiByte(0xFDE9u, 0, Buffer, buffer_len, 0LL, 0, 0LL, 0LL);
    cbMultiByte = buffer_mb_str;
    if ( buffer_mb_str )
    {
      if ( buffer_mb_str < 0x104 )
      {
        v8 = buffer_mb_str + 1;
        v9 = v8 + 15;
        if ( v8 + 15 <= v8 )
          v9 = 0xFFFFFFFFFFFFFF0LL;
        v10 = v9 & 0xFFFFFFFFFFFFFFF0uLL;
        v11 = alloca(v10);
        v12 = alloca(v10);
        WideCharToMultiByte(
          0xFDE9u,
          0,
          notification_data->Loaded.BaseDllName->Buffer,
          buffer_len,
          MultiByteStr,
          cbMultiByte,
          0LL,
          0LL);
        v14 = &off_141740A08;
        do
        {
          v15 = *v14;
          if ( MultiByteStr[0] )
          {
            v16 = (MultiByteStr - v15);
            while ( 1 )
            {
              v17 = *v15;
              if ( !v17 )
                break;
              v18 = v15[v16];
              if ( (v18 - 97) <= 0x19u )
                v18 -= 32;
              v13 = (v17 - 97);
              if ( (v17 - 97) <= 0x19u )
                LOBYTE(v17) = v17 - 32;
              if ( v18 < v17 || v18 > v17 )
                break;
              if ( !(++v15)[v16] )
                goto LABEL_19;
            }
          }
          else
          {
LABEL_19:
            if ( !*v15 && dword_141859918 != 1 )
            {
              for ( i = MultiByteStr; *i; ++i )
                ;
              if ( (i - MultiByteStr) < 260 )
              {
                set_gflags(v13, MultiByteStr);
                dword_141859918 = 1;
              }
            }
          }
          ++v5;
          ++v14;
        }
        while ( v5 < 3 );
      }
    }
  }
}
```