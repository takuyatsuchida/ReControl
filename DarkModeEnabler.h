#pragma once

#include <windows.h>

namespace DarkModeEnabler
{
    HHOOK RegisterDarkModeEnabler();
    bool UnregisterDarkModeEnabler();
} // namespace DarkModeEnabler
