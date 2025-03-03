#pragma once

#include <windows.h>

namespace ReControl
{
    HHOOK StartKeyInterceptor();
    bool StopKeyInterceptor();
} // namespace ReControl
