#pragma once
#include "pch.h"
#include "Config.h"

namespace Winshit
{
	//---------------------------------------------------------------------------
	template <typename T>
	void ProtWrite(T location, uint8_t shellcode[], size_t len)
	{
		DWORD prevProt;
		VirtualProtect(reinterpret_cast<LPVOID>(location), len, PAGE_EXECUTE_READWRITE, &prevProt);
		memcpy(reinterpret_cast<char*>(location), (char*)shellcode, 6);
		VirtualProtect(reinterpret_cast<char*>(location), len, prevProt, &prevProt);
	}
	//---------------------------------------------------------------------------
	template <typename T>
	void PrintAddress(String proc, T address)
	{
        #ifdef STDOUT_ADDRESSES
		std::cout << proc << "\t" << std::hex << address << "\n";
        #endif
	}
	//---------------------------------------------------------------------------
};
