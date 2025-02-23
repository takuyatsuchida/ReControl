#include "pch.h"
#include "MainWindow.xaml.h"
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

#define WM_NOTIFYICON (WM_APP + 1)

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::ReControl::implementation
{
    void MainWindow::OnActivated(const IInspectable &, const IInspectable &) const
    {
        this->AppWindow().Hide();
        AddNotifyIcon();
    }

    void MainWindow::OnClosed(const IInspectable &, const IInspectable &) const
    {
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
        const auto result = this->try_as<IWindowNative>()->get_WindowHandle(&hWnd);
        if (result == S_OK) return hWnd;

        const auto errorMessage = std::format(L"Failed to get window handle.\nError code: 0x{:08X}", result);
        MessageBox(nullptr, errorMessage.c_str(), L"Error", MB_OK | MB_ICONERROR);
        return nullptr;
    }

    LRESULT MainWindow::Subclassproc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam,
                                     UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
    {
        if (uMsg == WM_NOTIFYICON && lParam == WM_RBUTTONUP)
        {
            ShowNotifyIconMenu(hWnd);
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
        POINT pt;
        GetCursorPos(&pt);
        const auto hMenu = LoadMenu(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDR_NOTIFYICONMENU));
        const auto hSubMenu = GetSubMenu(hMenu, 0);
        SetForegroundWindow(hWnd);
        TrackPopupMenu(hSubMenu, TPM_LEFTBUTTON | TPM_LEFTALIGN | TPM_BOTTOMALIGN, pt.x, pt.y, 0, hWnd, nullptr);
        PostMessage(hWnd, WM_NULL, 0, 0);
    }
} // namespace winrt::ReControl::implementation
