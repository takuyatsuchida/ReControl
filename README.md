<div align="center">
  <img alt="icon" src="ReControl/ReControl.png">
  <h1>ReControl</h1>
  <a href="https://github.com/takuyatsuchida/ReControl/releases"><img alt="Release" src="https://img.shields.io/github/v/release/takuyatsuchida/ReControl?style=for-the-badge"></a>
  <a href="LICENSE"><img alt="License" src="https://img.shields.io/github/license/takuyatsuchida/ReControl?style=for-the-badge"></a>
</div>

## Overview

***Unify the behavior of the Control key between Windows and UNIX-based tools.***

**ReControl** is a Windows utility designed to provide a more integrated and efficient keyboard experience, inspired by the seamless environment of macOS where application shortcuts (Command key) and UNIX-based tool commands (Control key) coexist harmoniously.

## Features

* **Global Emacs-like Keybindings**: Use familiar Emacs navigation and editing shortcuts in almost any text field on Windows.
* **Smart Key Remapping**: Converts your CapsLock key into a Right Control key, providing an easily accessible, dedicated modifier key, much like the Control key's position on HHKB or Mac JIS keyboards.
* **Application Exclusion List**: Automatically disables ReControl's keybindings in applications where they are not needed, such as Windows Terminal, GNU Emacs, and gVim.

## Installation

> [!IMPORTANT]
> The installer will **not** overwrite an existing Scancode Map key in your registry.
> If you use other keyboard remapping tools (like Ctrl2Cap), you may need to remove your existing settings before installing ReControl.
>
> You can manually delete the key using Registry Editor at:\
> HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layout
>
> Alternatively, you can run the following command in PowerShell (as Administrator) to remove it:\
> `Remove-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Control\Keyboard Layout" -Name "Scancode Map"`
>

## Usage

Once installed, ReControl runs silently in the background and resides in the notification area.
The Emacs-like keybindings are immediately active system-wide.

### Global Keybindings

The following shortcuts are available.
They are triggered using the remapped CapsLock key (which now functions as RCtrl).

## License
This project is licensed under the [MIT License](LICENSE).
