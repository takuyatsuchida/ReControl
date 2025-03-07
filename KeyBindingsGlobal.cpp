#include "pch.h"
#include "KeyBindingsGlobal.h"

namespace ReControl::KeyBindings
{
    // Global key bindings are inspired by PSReadLine's Emacs key bindings
    // https://github.com/PowerShell/PSReadLine/blob/v2.3.6/PSReadLine/KeyBindings.cs#L267-L397
    bool ProcessKeyBindingsGlobal(const WPARAM wParam, const LPARAM lParam)
    {
        if (wParam == WM_KEYDOWN)
        {
            switch (const auto key = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam); key->vkCode)
            {
            case 'A': // RCtrl + A -> Home : BeginningOfLine
                if (!(GetAsyncKeyState(VK_RCONTROL) & 0x8000)) return false;
                MessageBox(nullptr, L"Ctrl + A", L"Key Binding", MB_OK | MB_ICONINFORMATION);
                return true;

            default:
                return false;
            }
        }
        return false;
    }
} // namespace ReControl::KeyBindings
