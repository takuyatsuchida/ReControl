#include "pch.h"
#include "KeyBindingsGlobal.h"

namespace
{
    constexpr DWORD KEYEVENTF_KEYDOWN = 0x0000;
} // namespace

namespace ReControl::KeyBindings
{
    // Global key bindings are inspired by PSReadLine's Emacs key bindings
    // https://github.com/PowerShell/PSReadLine/blob/v2.3.6/PSReadLine/KeyBindings.cs#L267-L397
    bool ProcessKeyBindingsGlobal(const WPARAM wParam, const LPARAM lParam)
    {
        if (wParam == WM_KEYDOWN)
        {
            switch (const auto key = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam); // NOLINT(performance-no-int-to-ptr)
                    key->vkCode)
            {
            case 'A': { // RControl + A -> Home : BeginningOfLine
                if (!(GetAsyncKeyState(VK_RCONTROL) & 0x8000)) return false;

                auto inputs = std::array{
                    // clang-format off
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYUP   | KEYEVENTF_EXTENDEDKEY}},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_HOME,     .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_HOME,     .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYDOWN | KEYEVENTF_EXTENDEDKEY}},
                    // clang-format on
                };
                SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));

                return true;
            }

            case 'B': { // RControl + B -> Left : BackwardChar
                if (!(GetAsyncKeyState(VK_RCONTROL) & 0x8000)) return false;

                auto inputs = std::array{
                    // clang-format off
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYUP   | KEYEVENTF_EXTENDEDKEY}},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_LEFT,     .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_LEFT,     .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYDOWN | KEYEVENTF_EXTENDEDKEY}},
                    // clang-format on
                };
                SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));

                return true;
            }

            case 'D': { // RControl + D -> Delete : DeleteChar
                if (!(GetAsyncKeyState(VK_RCONTROL) & 0x8000)) return false;

                auto inputs = std::array{
                    // clang-format off
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYUP   | KEYEVENTF_EXTENDEDKEY}},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_DELETE,   .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_DELETE,   .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYDOWN | KEYEVENTF_EXTENDEDKEY}},
                    // clang-format on
                };
                SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));

                return true;
            }

            case 'E': { // RControl + E -> End : EndOfLine
                if (!(GetAsyncKeyState(VK_RCONTROL) & 0x8000)) return false;

                auto inputs = std::array{
                    // clang-format off
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYUP   | KEYEVENTF_EXTENDEDKEY}},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_END,      .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_END,      .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYDOWN | KEYEVENTF_EXTENDEDKEY}},
                    // clang-format on
                };
                SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));

                return true;
            }

            case 'F': { // RControl + F -> Right : ForwardChar
                if (!(GetAsyncKeyState(VK_RCONTROL) & 0x8000)) return false;

                auto inputs = std::array{
                    // clang-format off
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYUP   | KEYEVENTF_EXTENDEDKEY}},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RIGHT,    .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RIGHT,    .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYDOWN | KEYEVENTF_EXTENDEDKEY}},
                    // clang-format on
                };
                SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));

                return true;
            }

            case 'H': { // RControl + H -> Backspace : BackwardDeleteChar
                if (!(GetAsyncKeyState(VK_RCONTROL) & 0x8000)) return false;

                auto inputs = std::array{
                    // clang-format off
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYUP   | KEYEVENTF_EXTENDEDKEY}},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_BACK,     .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_BACK,     .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYDOWN | KEYEVENTF_EXTENDEDKEY}},
                    // clang-format on
                };
                SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));

                return true;
            }

            case 'K': { // RControl + K -> Shift + End, Shift + Delete : KillLine
                if (!(GetAsyncKeyState(VK_RCONTROL) & 0x8000)) return false;

                auto inputs = std::array{
                    // clang-format off
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYUP   | KEYEVENTF_EXTENDEDKEY}},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_SHIFT,    .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_END,      .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_END,      .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_DELETE,   .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_DELETE,   .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_SHIFT,    .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYDOWN | KEYEVENTF_EXTENDEDKEY}},
                    // clang-format on
                };
                SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));

                return true;
            }

            case 'N': { // RControl + N -> Down : NextLine
                if (!(GetAsyncKeyState(VK_RCONTROL) & 0x8000)) return false;

                auto inputs = std::array{
                    // clang-format off
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYUP   | KEYEVENTF_EXTENDEDKEY}},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_DOWN,     .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_DOWN,     .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYDOWN | KEYEVENTF_EXTENDEDKEY}},
                    // clang-format on
                };
                SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));

                return true;
            }

            case 'P': { // RControl + P -> Up : PreviousLine
                if (!(GetAsyncKeyState(VK_RCONTROL) & 0x8000)) return false;

                auto inputs = std::array{
                    // clang-format off
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYUP   | KEYEVENTF_EXTENDEDKEY}},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_UP,       .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_UP,       .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYDOWN | KEYEVENTF_EXTENDEDKEY}},
                    // clang-format on
                };
                SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));

                return true;
            }

            case 'T': { // RControl + T -> Shift + Right, Shift + Delete, Left, Shift + Insert : SwapCharacters
                if (!(GetAsyncKeyState(VK_RCONTROL) & 0x8000)) return false;

                auto inputs = std::array{
                    // clang-format off
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYUP   | KEYEVENTF_EXTENDEDKEY}},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_SHIFT,    .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_LEFT,     .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_LEFT,     .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_DELETE,   .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_DELETE,   .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_SHIFT,    .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RIGHT,    .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RIGHT,    .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_SHIFT,    .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_INSERT,   .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_INSERT,   .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_SHIFT,    .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYDOWN | KEYEVENTF_EXTENDEDKEY}},
                    // clang-format on
                };
                SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));

                return true;
            }

            case 'Y': { // RControl + Y -> Shift + Insert : Yank
                if (!(GetAsyncKeyState(VK_RCONTROL) & 0x8000)) return false;

                auto inputs = std::array{
                    // clang-format off
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYUP   | KEYEVENTF_EXTENDEDKEY}},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_SHIFT,    .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_INSERT,   .dwFlags = KEYEVENTF_KEYDOWN                        }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_INSERT,   .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_SHIFT,    .dwFlags = KEYEVENTF_KEYUP                          }},
                    INPUT{.type = INPUT_KEYBOARD, .ki = {.wVk = VK_RCONTROL, .dwFlags = KEYEVENTF_KEYDOWN | KEYEVENTF_EXTENDEDKEY}},
                    // clang-format on
                };
                SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));

                return true;
            }

            default:
                return false;
            }
        }

        return false;
    }
} // namespace ReControl::KeyBindings
