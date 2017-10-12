///
/// Talk is cheap. Show me the code.    - Linus Torvalds
///
/// Author: WiNGC (wingc@wingc.net)
/// All rights reserved
///

#pragma once

#include <Windows.h>
#include <unique_handle.hxx>

struct FileHandleTraits
{
    static HANDLE invalid() throw() { return INVALID_HANDLE_VALUE; };
    static void close(HANDLE value) throw() { ::CloseHandle(value); };
};

struct HeapTraits
{
    static PVOID invalid() throw() { return nullptr; };
    static void close(PVOID value) throw() { ::HeapFree(::GetProcessHeap(), 0, value); };
};

typedef unique_handle<HANDLE, FileHandleTraits> unqiue_FileHandle;
typedef unique_handle<PVOID, HeapTraits> unique_Heap;