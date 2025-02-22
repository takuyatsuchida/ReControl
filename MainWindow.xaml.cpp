#include "pch.h"
#include "MainWindow.xaml.h"
#include "resource.h"
#include <format>
#include <microsoft.ui.xaml.window.h>
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#pragma comment(lib, "Shell32.lib")

namespace winrt::ReControl::implementation
{
    MainWindow::MainWindow() : MainWindowT()
    {
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
        nid.uFlags = NIF_ICON | NIF_TIP;
        nid.hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_APPICON));
        wcscpy_s(nid.szTip, L"ReControl");
        Shell_NotifyIcon(NIM_ADD, &nid);
    }

    void MainWindow::DeleteNotifyIcon() const
    {
        NOTIFYICONDATA nid = {};
        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.hWnd = GetWindowHandle();
        nid.uID = 1;
        Shell_NotifyIcon(NIM_DELETE, &nid);
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
} // namespace winrt::ReControl::implementation
