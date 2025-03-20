#pragma once

#include <windows.h>

namespace ReControl
{
    HHOOK RegisterDarkModeEnabler();
    bool UnregisterDarkModeEnabler();
} // namespace ReControl
