<div align="center">
  <img alt="icon" src="ReControl/ReControl.png">
  <h1>ReControl</h1>
  <a href="https://github.com/takuyatsuchida/ReControl/releases"><img alt="Releases" src="https://img.shields.io/github/v/release/takuyatsuchida/ReControl?style=for-the-badge"></a>
  <a href="LICENSE"><img alt="License" src="https://img.shields.io/github/license/takuyatsuchida/ReControl?style=for-the-badge"></a>
</div>

## Overview

***Unify the behavior of the Control key between Windows and UNIX-based tools.***

**ReControl** is a Windows utility designed to deliver an integrated and efficient keyboard experience modeled on macOS, where keyboard shortcuts with <kbd>Command</kbd> and key bindings in UNIX-based tools with <kbd>Control</kbd> coexist harmoniously.

This application enables **global Emacs-like key bindings** across your Windows system by remapping <kbd>CapsLock</kbd> to <kbd>RCtrl</kbd> (Right Ctrl).

This creates a clear separation of concerns. <kbd>RCtrl</kbd> handles ReControl’s global Emacs-like key bindings, similar to <kbd>Control</kbd> on macOS. The standard <kbd>Ctrl</kbd> (Left Ctrl) handles system and application shortcuts, similar to <kbd>Command</kbd> on macOS. The result is a fluid, conflict-free experience.

## Features

- **Global Emacs-like key bindings**: Emacs-style navigation and editing in most text fields across Windows.
- **Smart key remapping**: Remaps <kbd>CapsLock</kbd> to <kbd>RCtrl</kbd> as ReControl’s modifier.
- **Automatic application exclusions**: Automatically disabled in terminals and editors to prevent conflicts.

## Installation

1.  Download the latest installer `ReControl-x.x.x-x64.msi` from the [**Releases**](https://github.com/takuyatsuchida/ReControl/releases) page.
2.  Run the downloaded `.msi` file to install the application.
3.  Restart Windows to apply the key remapping.

The installer will:
* Place the executable in `C:\Program Files\ReControl`.
* Add the application to your startup programs through a registry key so that it runs automatically when Windows starts.
* Create a `Scancode Map` entry in the Windows registry to remap <kbd>CapsLock</kbd> to <kbd>RCtrl</kbd>.

> [!IMPORTANT]
>
> The installer will **NOT** overwrite any existing `Scancode Map` key in your registry. If you use other keyboard remapping tools such as **Ctrl2Cap**, you may need to remove or configure them yourself before installing ReControl.
>
> You can manually delete the `Scancode Map` key using **Registry Editor** at: `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layout`
>
> Alternatively, run the following command in **PowerShell** as an Administrator to remove it:
>
> ```powershell
> Remove-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Control\Keyboard Layout" -Name "Scancode Map"
> ```
>
> A system restart is required for the changes to take effect.
>

### Uninstallation

You can uninstall ReControl at any time from **Settings > Apps > Installed apps** in Windows. The uninstaller will also remove the registry key that was created during installation.

## Usage

Once installed, ReControl runs silently in the background and resides in the notification area. Its Emacs-like key bindings become active system-wide immediately.

### Global Key Bindings

The following shortcuts are available. They are triggered by the remapped <kbd>CapsLock</kbd>, which now acts as <kbd>RCtrl</kbd>.

| Shortcut                      | Action                            | Equivalent Keys                                                                                                                  |
| :---------------------------- | :-------------------------------- | :------------------------------------------------------------------------------------------------------------------------------- |
| <kbd>RCtrl</kbd>+<kbd>A</kbd> | Move to the beginning of the line | <kbd>Home</kbd>                                                                                                                  |
| <kbd>RCtrl</kbd>+<kbd>B</kbd> | Move left by one character        | <kbd>Left</kbd>                                                                                                                  |
| <kbd>RCtrl</kbd>+<kbd>D</kbd> | Delete the character to the right | <kbd>Delete</kbd>                                                                                                                |
| <kbd>RCtrl</kbd>+<kbd>E</kbd> | Move to the end of the line       | <kbd>End</kbd>                                                                                                                   |
| <kbd>RCtrl</kbd>+<kbd>F</kbd> | Move right by one character       | <kbd>Right</kbd>                                                                                                                 |
| <kbd>RCtrl</kbd>+<kbd>H</kbd> | Delete the character to the left  | <kbd>Backspace</kbd>                                                                                                             |
| <kbd>RCtrl</kbd>+<kbd>K</kbd> | Cut text to the end of the line   | <kbd>Shift</kbd>+<kbd>End</kbd> -> <kbd>Shift</kbd>+<kbd>Delete</kbd>                                                            |
| <kbd>RCtrl</kbd>+<kbd>N</kbd> | Move to the next line             | <kbd>Down</kbd>                                                                                                                  |
| <kbd>RCtrl</kbd>+<kbd>P</kbd> | Move to the previous line         | <kbd>Up</kbd>                                                                                                                    |
| <kbd>RCtrl</kbd>+<kbd>T</kbd> | Transpose two characters          | <kbd>Shift</kbd>+<kbd>Left</kbd> -> <kbd>Shift</kbd>+<kbd>Delete</kbd> -> <kbd>Right</kbd> -> <kbd>Shift</kbd>+<kbd>Insert</kbd> |
| <kbd>RCtrl</kbd>+<kbd>Y</kbd> | Paste text                        | <kbd>Shift</kbd>+<kbd>Insert</kbd>                                                                                               |

> [!WARNING]
>
> <kbd>RCtrl</kbd>+<kbd>K</kbd> and <kbd>RCtrl</kbd>+<kbd>T</kbd> will overwrite the current contents of your clipboard.
>

### Excluded Applications

To prevent conflicts, ReControl is disabled by default for the following applications:

* Windows Terminal (`WindowsTerminal.exe`)
* GNU Emacs (`emacs.exe`)
* gVim (`gvim.exe`)

## License

This project is licensed under the [MIT License](LICENSE).
