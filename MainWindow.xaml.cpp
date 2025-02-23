#include "pch.h"
#include "MainWindow.xaml.h"
#include "KeyInterceptor.h"
#include "resource.h"
#include <commctrl.h>
#include <format>
#include <microsoft.ui.xaml.window.h>
#include <winrt/Microsoft.UI.Windowing.h>
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Version.lib")

#define WM_NOTIFYICON (WM_APP + 1)

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::ReControl::implementation
{
    void MainWindow::OnActivated(const IInspectable &, const IInspectable &) const
    {
        this->AppWindow().Hide();
        ApplySystemDarkModeToMenu();
        AddNotifyIcon();
        StartKeyInterceptor();
    }

    void MainWindow::OnClosed(const IInspectable &, const IInspectable &) const
    {
        StopKeyInterceptor();
        DeleteNotifyIcon();
    }

    void MainWindow::AddNotifyIcon() const
    {
        NOTIFYICONDATA nid = {};
        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.hWnd = GetWindowHandle();
        nid.uID = 1;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        nid.uCallbackMessage = WM_NOTIFYICON;
        nid.hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_APPICON));
        wcscpy_s(nid.szTip, L"ReControl");
        Shell_NotifyIcon(NIM_ADD, &nid);
        SetWindowSubclass(GetWindowHandle(), Subclassproc, 1, reinterpret_cast<DWORD_PTR>(this));
    }

    void MainWindow::DeleteNotifyIcon() const
    {
        NOTIFYICONDATA nid = {};
        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.hWnd = GetWindowHandle();
        nid.uID = 1;
        Shell_NotifyIcon(NIM_DELETE, &nid);
        RemoveWindowSubclass(GetWindowHandle(), Subclassproc, 1);
    }

    HWND MainWindow::GetWindowHandle() const
    {
        HWND hWnd = nullptr;
        const auto hResult = this->try_as<IWindowNative>()->get_WindowHandle(&hWnd);
        if (hResult == S_OK) return hWnd;

        const auto message = std::format(L"Failed to get window handle.\nError code: 0x{:08X}", hResult);
        MessageBox(nullptr, message.c_str(), L"Error", MB_OK | MB_ICONERROR);
        return nullptr;
    }

    LRESULT MainWindow::Subclassproc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam,
                                     UINT_PTR, DWORD_PTR)
    {
        if (uMsg == WM_NOTIFYICON && lParam == WM_RBUTTONUP)
        {
            ShowNotifyIconMenu(hWnd);
            return 0;
        }

        if (uMsg == WM_COMMAND && wParam == ID_NOTIFYICONMENU_ABOUT)
        {
            ShowAboutDialog();
            return 0;
        }

        if (uMsg == WM_COMMAND && wParam == ID_NOTIFYICONMENU_EXIT)
        {
            Application::Current().Exit();
            return 0;
        }

        return DefSubclassProc(hWnd, uMsg, wParam, lParam);
    }

    void MainWindow::ShowNotifyIconMenu(const HWND hWnd)
    {
        POINT point;
        GetCursorPos(&point);
        const auto hMenu = LoadMenu(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDR_NOTIFYICONMENU));
        const auto hSubMenu = GetSubMenu(hMenu, 0);
        SetForegroundWindow(hWnd);
        TrackPopupMenu(hSubMenu, TPM_LEFTBUTTON | TPM_LEFTALIGN | TPM_BOTTOMALIGN, point.x, point.y, 0, hWnd, nullptr);
        DestroyMenu(hMenu);
        PostMessage(hWnd, WM_NULL, 0, 0);
    }

    void MainWindow::ShowAboutDialog()
    {
        const auto message = std::format(L"{} {}\n{}", GetVersionInfo(L"ProductName"),
                                         GetVersionInfo(L"ProductVersion"), GetVersionInfo(L"LegalCopyright"));
        MessageBox(nullptr, message.c_str(), L"About", MB_OK | MB_ICONINFORMATION);
    }

    std::wstring MainWindow::GetVersionInfo(const std::wstring &key)
    {
        WCHAR fileName[MAX_PATH];
        GetModuleFileName(nullptr, fileName, MAX_PATH);

        const auto size = GetFileVersionInfoSize(fileName, nullptr);

        std::vector<BYTE> data(size);
        GetFileVersionInfo(fileName, 0, size, data.data());

        void *buffer = nullptr;
        UINT length = 0;
        const auto query = L"\\StringFileInfo\\040904b0\\" + key; // English (United States)
        VerQueryValue(data.data(), query.c_str(), &buffer, &length);

        return {static_cast<wchar_t *>(buffer)};
    }

    void MainWindow::StartKeyInterceptor()
    {
        if (const auto hHook = KeyInterceptor::StartKeyInterceptor(); hHook != nullptr) return;

        NotifyLastError(L"Failed to start Key Interceptor.\nError: {}");
        Application::Current().Exit();
    }

    void MainWindow::StopKeyInterceptor()
    {
        if (const auto result = KeyInterceptor::StopKeyInterceptor(); !result)
            NotifyLastError(L"Failed to stop Key Interceptor.\nError: {}");
    }

    void MainWindow::NotifyLastError(const std::wstring &errorFormat)
    {
        const auto errorMessage = KeyInterceptor::GetLastErrorMessage();
        const std::wstring message = std::vformat(errorFormat, std::make_wformat_args(errorMessage));
        MessageBox(nullptr, message.c_str(), L"Error", MB_OK | MB_ICONERROR);
        KeyInterceptor::FreeLastErrorMessage(errorMessage);
    }

    void MainWindow::ApplySystemDarkModeToMenu()
    {
        enum class PreferredAppMode // NOLINT(performance-enum-size)
        {
            Default,
            AllowDark,
            ForceDark,
            ForceLight
        };

        const HMODULE hModule = LoadLibraryEx(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (!hModule) return;

        const auto setPreferredAppMode = reinterpret_cast<PreferredAppMode(WINAPI *)(PreferredAppMode appMode)>(
            GetProcAddress(hModule, MAKEINTRESOURCEA(135)));
        if (!setPreferredAppMode) return;

        setPreferredAppMode(PreferredAppMode::AllowDark);

        FreeLibrary(hModule);
    }
} // namespace winrt::ReControl::implementation
