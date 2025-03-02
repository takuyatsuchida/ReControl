#pragma once

#include <windows.h>

namespace KeyInterceptor
{
    HHOOK StartKeyInterceptor();
    bool StopKeyInterceptor();
} // namespace KeyInterceptor
