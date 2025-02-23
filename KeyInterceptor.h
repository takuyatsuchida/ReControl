#pragma once

#include <windows.h>

namespace KeyInterceptor
{
    HHOOK StartKeyInterceptor();
    bool StopKeyInterceptor();
    wchar_t *GetLastErrorMessage();
    void FreeLastErrorMessage(wchar_t *message);
} // namespace KeyInterceptor
