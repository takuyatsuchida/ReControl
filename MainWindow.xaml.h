#pragma once

#include "MainWindow.g.h"

namespace winrt::ReControl::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();
        void OnClosed(const IInspectable &, const IInspectable &) const;

      private:
        void AddNotifyIcon() const;
        void DeleteNotifyIcon() const;
        [[nodiscard]] HWND GetWindowHandle() const;
    };
} // namespace winrt::ReControl::implementation

namespace winrt::ReControl::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
} // namespace winrt::ReControl::factory_implementation
