#include "pch.h"
#include "DarkModeEnabler.h"
#include <dwmapi.h>
#include <shlwapi.h>
#include <uxtheme.h>

#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "UxTheme.lib")

namespace
{
    enum class PreferredAppMode // NOLINT(performance-enum-size)
    {
        AllowDark = 1
    };

    typedef bool(WINAPI *ShouldAppsUseDarkMode)();
    typedef PreferredAppMode(WINAPI *SetPreferredAppMode)(PreferredAppMode);

    ShouldAppsUseDarkMode shouldAppsUseDarkMode = nullptr;
    SetPreferredAppMode setPreferredAppMode = nullptr;

    constexpr auto FOREGROUND_COLOR = RGB(255, 255, 255);
    constexpr auto BACKGROUND_COLOR = RGB(32, 32, 32);

    HBRUSH hbrBackground = nullptr;
    HHOOK hCBTHook = nullptr;
    std::vector<HWND> hookedHWndList;

    void InitializeUxThemeDarkModeFunctions()
    {
        if (const auto hModule = LoadLibrary(L"UxTheme.dll"))
        {
            shouldAppsUseDarkMode =
                reinterpret_cast<ShouldAppsUseDarkMode>(GetProcAddress(hModule, MAKEINTRESOURCEA(132)));
            setPreferredAppMode = reinterpret_cast<SetPreferredAppMode>(GetProcAddress(hModule, MAKEINTRESOURCEA(135)));
            FreeLibrary(hModule);
        }
    }

    void SetDarkMode(const HWND hWnd)
    {
        const auto darkMode = shouldAppsUseDarkMode() ? TRUE : FALSE;
        DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &darkMode, sizeof(darkMode));
    }

    void SetDarkModeToChildWindows(const HWND hWnd)
    {
        HWND child = nullptr;
        while ((child = FindWindowEx(hWnd, child, nullptr, nullptr)))
        {
            WCHAR className[MAX_CLASS_NAME];
            GetClassName(child, className, MAX_CLASS_NAME);
            if (PathMatchSpec(className, WC_BUTTON))
            {
                SetWindowTheme(child, shouldAppsUseDarkMode() ? L"DarkMode_Explorer" : L"Explorer", nullptr);
            }
            SetDarkModeToChildWindows(child);
        }
    }

    LRESULT CALLBACK Subclassproc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam, UINT_PTR,
                                  DWORD_PTR)
    {
        switch (uMsg)
        {
        case WM_INITDIALOG:
        case WM_SETTINGCHANGE:
            SetDarkMode(hWnd);
            SetDarkModeToChildWindows(hWnd);
            RedrawWindow(hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_ALLCHILDREN);
            break;

        case WM_CTLCOLORBTN:
        case WM_CTLCOLORSTATIC:
            if (!shouldAppsUseDarkMode()) break;

            SetTextColor(reinterpret_cast<HDC>(wParam), FOREGROUND_COLOR); // NOLINT(performance-no-int-to-ptr)
            SetBkColor(reinterpret_cast<HDC>(wParam), BACKGROUND_COLOR);   // NOLINT(performance-no-int-to-ptr)
            return reinterpret_cast<LRESULT>(hbrBackground);

        case WM_ERASEBKGND:
            if (!shouldAppsUseDarkMode()) break;

            RECT rect;
            GetClientRect(hWnd, &rect);
            FillRect(reinterpret_cast<HDC>(wParam), &rect, hbrBackground); // NOLINT(performance-no-int-to-ptr)
            return 1;

        case WM_PAINT:
            if (!shouldAppsUseDarkMode()) break;

            WCHAR className[MAX_CLASS_NAME];
            GetClassName(hWnd, className, MAX_CLASS_NAME);
            if (PathMatchSpec(className, L"#32770"))
            {
                PAINTSTRUCT paint;
                BeginPaint(hWnd, &paint);
                EndPaint(hWnd, &paint);
                return 0;
            }
            break;

        default:
            break;
        }

        return DefSubclassProc(hWnd, uMsg, wParam, lParam);
    }

    void SetDarkModeToWindow(const HWND hWnd)
    {
        if (std::ranges::find(hookedHWndList, hWnd) != hookedHWndList.end()) return;

        hookedHWndList.push_back(hWnd);
        SetWindowSubclass(hWnd, Subclassproc, reinterpret_cast<UINT_PTR>(Subclassproc), 0);
    }

    void RemoveDarkModeFromWindow(const HWND hWnd)
    {
        if (const auto it = std::ranges::find(hookedHWndList, hWnd); it != hookedHWndList.end())
        {
            hookedHWndList.erase(it);
            RemoveWindowSubclass(hWnd, Subclassproc, reinterpret_cast<UINT_PTR>(Subclassproc));
        }
    }

    LRESULT CALLBACK CBTProc(const int nCode, const WPARAM wParam, const LPARAM lParam)
    {
        switch (nCode)
        {
        case HCBT_CREATEWND: {
            const auto hWnd = reinterpret_cast<HWND>(wParam); // NOLINT(performance-no-int-to-ptr)
            WCHAR className[MAX_CLASS_NAME];
            GetClassName(hWnd, className, MAX_CLASS_NAME);
            if (PathMatchSpec(className, L"#32770"))
            {
                SetDarkModeToWindow(hWnd);
            }
            return 0;
        }

        case HCBT_DESTROYWND:
            RemoveDarkModeFromWindow(reinterpret_cast<HWND>(wParam)); // NOLINT(performance-no-int-to-ptr)
            return 0;

        default:
            break;
        }

        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    }
} // namespace

namespace DarkModeEnabler
{
    HHOOK RegisterDarkModeEnabler()
    {
        if (hCBTHook) return hCBTHook;

        InitializeUxThemeDarkModeFunctions();
        setPreferredAppMode(PreferredAppMode::AllowDark);

        hbrBackground = CreateSolidBrush(BACKGROUND_COLOR);

        hCBTHook = SetWindowsHookEx(WH_CBT, CBTProc, nullptr, GetCurrentThreadId());
        return hCBTHook;
    }

    bool UnregisterDarkModeEnabler()
    {
        if (!hCBTHook) return true;

        const auto bResult = UnhookWindowsHookEx(hCBTHook);
        if (bResult)
        {
            hCBTHook = nullptr;
        }

        for (const auto hWnd : hookedHWndList)
        {
            RemoveWindowSubclass(hWnd, Subclassproc, reinterpret_cast<UINT_PTR>(Subclassproc));
        }
        hookedHWndList.clear();

        if (hbrBackground)
        {
            DeleteObject(hbrBackground);
            hbrBackground = nullptr;
        }

        return bResult;
    }
} // namespace DarkModeEnabler
