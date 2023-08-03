#include "pch.h"
#include "Patches.h"
#include <iostream>
#include "Winshit.h"

namespace Patches
{
	//---------------------------------------------------------------------------
	void QuerySysInfo()
	{
	}
	//---------------------------------------------------------------------------
	void DebuggerPresent()
	{
		const auto k32 = GetModuleHandle("KernelBase");
		const auto isDebuggerPresentLoc = GetProcAddress(k32, "IsDebuggerPresent");

		/*
			Original disasm is something like
			----------------------------------
			mov rax, gs:[00000060]       
			mov eax, qword ptr[rax+2]    ; teb + 0x2 = `BeingDebugged`
			ret

			Which I **think** is something like `NtCurrentTeb()->someproperty`, with `NtGetCurrentPeb` being inlined.
			Windows stores the TEB in GS on x64, FS on x86. Smarter every day and that.
		*/
		uint8_t shellcode[]
		{
			0xB8, 0x00, 0x00, 0x00, 0x00,        // mov eax, 0
			0xC3                                 // ret
		};
		Winshit::ProtWrite<FARPROC>(isDebuggerPresentLoc, shellcode, 6);
		Winshit::PrintAddress<FARPROC>("IsDebuggerPresent", isDebuggerPresentLoc);
	}
	//---------------------------------------------------------------------------
}
