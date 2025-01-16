// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <cstdint>
#include <string>
#include <iostream>

// Microsoft's caps lock key broken
using Handle = HANDLE;
using ThreadEntry = THREADENTRY32;

template <typename T>
using Vector = std::vector<T>;
using String = std::string;

// For those with weak pinky fingers
using uint32 = uint32_t;
using uintptr = uintptr_t;

#define PRINT_LAST_ERROR(src) \
    do { \
        DWORD errorCode = GetLastError(); \
        char* errorMessage = nullptr; \
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, \
                       nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
                       (LPSTR)&errorMessage, 0, nullptr); \
        std::cout << "[" << src << "] Error: " << errorCode << ": " << errorMessage << "\n"; \
        LocalFree(errorMessage); \
    } while (false)
#endif //PCH_H
