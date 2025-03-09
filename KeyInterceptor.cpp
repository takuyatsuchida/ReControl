#include "pch.h"
#include "KeyInterceptor.h"
#include "KeyBindingsGlobal.h"
#include <shlwapi.h>

namespace
{
    HHOOK hHook = nullptr;

    const std::vector<std::wstring> EXCLUSION_LIST = {L"WindowsTerminal.exe"};

    bool IsForegroundApplicationExcluded()
    {
        const auto hWnd = GetForegroundWindow();
        if (!hWnd) return false;

        DWORD processId = 0;
        GetWindowThreadProcessId(hWnd, &processId);
        if (!processId) return false;

        const auto hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);
        if (!hProcess) return false;

        WCHAR exePath[MAX_PATH]{};
        DWORD size = MAX_PATH;
        const auto bResult = QueryFullProcessImageName(hProcess, 0, exePath, &size);
        CloseHandle(hProcess);
        if (!bResult) return false;

        const auto exeName = PathFindFileName(exePath);

        return std::ranges::find(EXCLUSION_LIST, exeName) != EXCLUSION_LIST.end();
    }

    LRESULT CALLBACK LowLevelKeyboardProc(const int nCode, const WPARAM wParam, const LPARAM lParam)
    {
        if (nCode == HC_ACTION)
        {
            if (IsForegroundApplicationExcluded()) return CallNextHookEx(nullptr, nCode, wParam, lParam);

            if (ReControl::KeyBindings::ProcessKeyBindingsGlobal(wParam, lParam)) return 1;
        }

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
