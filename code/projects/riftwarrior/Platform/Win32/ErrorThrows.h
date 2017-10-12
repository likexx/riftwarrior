///
/// Talk is cheap. Show me the code.    - Linus Torvalds
///
/// Author: WiNGC (wingc@wingc.net)
/// All rights reserved
///

#pragma once

#include <Windows.h>

namespace Util
{
struct CheckFailed
{
    size_t error_code;
    explicit CheckFailed(const size_t result) : error_code(result) {};
};

inline void Check(bool result)
{
    if (!result)
    {
        throw CheckFailed(0);
    }
}

inline void CheckWin32Error(bool result)
{
    if (!result)
    {
        throw CheckFailed(HRESULT_FROM_WIN32(::GetLastError()));
    }
}

inline void CheckParameter(bool result)
{
    if (!result)
    {
        throw CheckFailed(E_INVALIDARG);
    }
}


inline void Check(BOOL result)
{
    if (!result)
    {
        throw CheckFailed(HRESULT_FROM_WIN32(::GetLastError()));
    }
}

inline void Check(HRESULT result)
{
    if (FAILED(result))
    {
        throw CheckFailed(result);
    }
}

template <typename T>
inline void Check(const T* pT)
{
    if (pT == nullptr)
    {
        throw std::bad_alloc;
    }
}
};

#ifdef _DEBUG
#include <CrtDbg.h>
// TODO: Better to have adaptive logging which means it can hook up with variant loggings by different systems.
#define _NUM2STR(x) #x
#define __AT__ __FILEW__ L":" _STR2WSTR(_NUM2STR(__LINE__))
#define ASSERT(_expression) _ASSERTE(_expression)
#define VERIFY(_expression) ASSERT(_expression)
#define VERIFY_VALUE(_expected, _expression)    ASSERT(_expected == _expression)
#define CHECK(_expression) \
    try { Util::Check(_expression); } catch(...) { ::OutputDebugString(L"Check failed (" __AT__ L")!"); throw; }
#define CHECK_WIN32ERR(_expression) \
    try { Util::CheckWin32Error(_expression); } catch(...) { ::OutputDebugString(L"Check failed (" __AT__ L")!"); throw; }
#define VALID_PARAM(_expression) \
    try { Util::CheckParameter(_expression); } catch(...) { ::OutputDebugString(L"Parameter is unexpected (" __AT__ L")!"); throw; }
#else
#define ASSERT(_expression) ((void)0)
#define VERIFY(_expression) (_expression)
#define VERIFY_VALUE(_expected, _expression)    (_expression)
#define CHECK(_expression)                      Util::Check(_expression)
#define CHECK_WIN32ERR(_expression)             Util::CheckWin32Error(_expression)
#define VALID_PARAM(_expression)                Util::CheckParameter(_expression)
#endif  // _DEBUG
