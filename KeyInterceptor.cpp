#include "pch.h"
#include "KeyInterceptor.h"

namespace
{
    HHOOK hHook = nullptr;

    LRESULT CALLBACK LowLevelKeyboardProc(const int nCode, const WPARAM wParam, const LPARAM lParam)
    {
        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    }
} // namespace

namespace ReControl
{
    HHOOK StartKeyInterceptor()
    {
        if (hHook) return hHook;

        hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, nullptr, 0);

        return hHook;
    }

    bool StopKeyInterceptor()
    {
        if (!hHook) return true;

        const auto bResult = UnhookWindowsHookEx(hHook);
        if (bResult)
        {
            hHook = nullptr;
        }

        return bResult;
    }
} // namespace ReControl
