#include "pch.h"
#include "KeyInterceptor.h"
#include "KeyBindingsGlobal.h"
#include <shlwapi.h>

namespace
{
    const std::vector<std::wstring> EXCLUSION_LIST = {L"WindowsTerminal.exe", L"emacs.exe", L"gvim.exe"};

    HHOOK hHook = nullptr;

    std::wstring GetForegroundApplicationExeName()
    {
        const auto hWnd = GetForegroundWindow();
        if (!hWnd) return {};

        DWORD processId = 0;
        GetWindowThreadProcessId(hWnd, &processId);
        if (!processId) return {};

        const auto hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);
        if (!hProcess) return {};

        WCHAR exePath[MAX_PATH];
        DWORD size = MAX_PATH;
        const auto bResult = QueryFullProcessImageName(hProcess, 0, exePath, &size);
        CloseHandle(hProcess);
        if (!bResult) return {};

        return PathFindFileName(exePath);
    }

    bool IsForegroundApplicationExcluded(const std::wstring &exeName)
    {
        return std::ranges::find(EXCLUSION_LIST, exeName) != EXCLUSION_LIST.end();
    }

    LRESULT CALLBACK LowLevelKeyboardProc(const int nCode, const WPARAM wParam, const LPARAM lParam)
    {
        if (nCode != HC_ACTION) return CallNextHookEx(nullptr, nCode, wParam, lParam);

        const auto exeName = GetForegroundApplicationExeName();
        if (IsForegroundApplicationExcluded(exeName)) return CallNextHookEx(nullptr, nCode, wParam, lParam);

        if (ReControl::KeyBindings::ProcessKeyBindingsGlobal(wParam, lParam)) return 1;

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
