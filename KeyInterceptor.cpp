#include "pch.h"
#include "KeyInterceptor.h"
#include <format>

namespace
{
    HHOOK hHook = nullptr;

    LRESULT LowLevelKeyboardProc(const int code, const WPARAM wParam, const LPARAM lParam)
    {
        return CallNextHookEx(nullptr, code, wParam, lParam);
    }
} // namespace

namespace KeyInterceptor
{
    HHOOK StartKeyInterceptor()
    {
        if (hHook != nullptr) return hHook;

        hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, nullptr, 0);

        return hHook;
    }

    bool StopKeyInterceptor()
    {
        if (hHook == nullptr) return true;

        const auto bResult = UnhookWindowsHookEx(hHook);
        if (bResult) hHook = nullptr;

        return bResult;
    }

    wchar_t *GetLastErrorMessage()
    {
        const auto dwCode = GetLastError();

        LPWSTR lpMessage = nullptr;
        const auto dwSize =
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                          nullptr, dwCode, 0, reinterpret_cast<LPWSTR>(&lpMessage), 0, nullptr);
        const auto message = std::format(L"{} - {}", dwCode, dwSize ? lpMessage : L"Unknown error.");
        LocalFree(lpMessage);

        return _wcsdup(message.c_str());
    }

    void FreeLastErrorMessage(wchar_t *message)
    {
        free(message);
    }
} // namespace KeyInterceptor
