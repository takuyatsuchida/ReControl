#pragma once

#include "MainWindow.g.h"

namespace winrt::ReControl::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow() = default;
        void OnActivated(const IInspectable &, const IInspectable &) const;
        void OnClosed(const IInspectable &, const IInspectable &) const;

      private:
        void AddNotifyIcon() const;
        void DeleteNotifyIcon() const;
        [[nodiscard]] HWND GetWindowHandle() const;
        static LRESULT Subclassproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass,
                                    DWORD_PTR dwRefData);
        static void ShowNotifyIconMenu(HWND hWnd);
        static void ShowAboutDialog();
        static std::wstring GetVersionInfo(const std::wstring &key);
        static void StartKeyInterceptor();
        static void StopKeyInterceptor();
        static void NotifyLastError(const std::wstring &errorFormat);
        static void ApplySystemDarkModeToMenu();
    };
} // namespace winrt::ReControl::implementation

namespace winrt::ReControl::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
} // namespace winrt::ReControl::factory_implementation
