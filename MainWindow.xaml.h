#pragma once

#include "MainWindow.g.h"

namespace winrt::ReControl::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow() = default;
    };
} // namespace winrt::ReControl::implementation

namespace winrt::ReControl::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
} // namespace winrt::ReControl::factory_implementation
