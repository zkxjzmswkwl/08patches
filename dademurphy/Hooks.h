#pragma once
#include "pch.h"
#include <iomanip>

using fn = uintptr_t * (*)(...);

fn oSend;
void __fastcall SendHook(void* socket, const char* buf, int len, int flags)
{
	printf("[%d] ", len);
	for (int i = len; i >= 0; --i)
	{
		printf("%02hhx ", (int)buf[i]);
	}
	printf("\n");
	oSend(socket, buf, len, flags);
}
