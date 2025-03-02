#include "pch.h"
#include "NotifyIcon.h"
#include "KeyInterceptor.h"
#include "resource.h"

#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Version.lib")

namespace
{
    constexpr auto WM_NOTIFYICON = WM_APP + 1;

    void AddNotifyIcon(const HWND hWnd)
    {
        NOTIFYICONDATA nid = {};
        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.hWnd = hWnd;
        nid.uID = 1;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        nid.uCallbackMessage = WM_NOTIFYICON;
        nid.hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_APPICON));
        wcscpy_s(nid.szTip, L"ReControl");
        Shell_NotifyIcon(NIM_ADD, &nid);
    }

    void DeleteNotifyIcon(const HWND hWnd)
    {
        NOTIFYICONDATA nid = {};
        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.hWnd = hWnd;
        nid.uID = 1;
        Shell_NotifyIcon(NIM_DELETE, &nid);
    }

    void ShowNotifyIconMenu(const HWND hWnd)
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

    std::wstring GetVersionInfo(const std::wstring &key)
    {
        WCHAR fileName[MAX_PATH];
        GetModuleFileName(nullptr, fileName, MAX_PATH);

        const auto dwLen = GetFileVersionInfoSize(fileName, nullptr);

        std::vector<BYTE> data(dwLen);
        GetFileVersionInfo(fileName, 0, dwLen, data.data());

        LPVOID lpBuffer = nullptr;
        UINT uLen = 0;
        const auto query = L"\\StringFileInfo\\040904b0\\" + key; // English (United States)
        VerQueryValue(data.data(), query.c_str(), &lpBuffer, &uLen);

        return {static_cast<wchar_t *>(lpBuffer)};
    }

    void ShowAboutDialog()
    {
        const auto message = std::format(L"{} {}\n{}", GetVersionInfo(L"ProductName"),
                                         GetVersionInfo(L"ProductVersion"), GetVersionInfo(L"LegalCopyright"));
        MessageBox(nullptr, message.c_str(), L"About", MB_OK | MB_ICONINFORMATION);
    }

    void NotifyLastError(const std::wstring &errorFormat)
    {
        const auto dwCode = GetLastError();

        LPWSTR lpMessage = nullptr;
        const auto dwSize =
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                          nullptr, dwCode, 0, reinterpret_cast<LPWSTR>(&lpMessage), 0, nullptr);
        const auto errorMessage = std::format(L"{} - {}", dwCode, dwSize ? lpMessage : L"Unknown error.");
        LocalFree(lpMessage);

        const auto message = std::vformat(errorFormat, std::make_wformat_args(errorMessage));
        MessageBox(nullptr, message.c_str(), L"Error", MB_OK | MB_ICONERROR);
    }

    void StartKeyInterceptor()
    {
        if (KeyInterceptor::StartKeyInterceptor()) return;

        NotifyLastError(L"Failed to start Key Interceptor.\nError: {}");
    }

    void StopKeyInterceptor()
    {
        if (KeyInterceptor::StopKeyInterceptor()) return;

        NotifyLastError(L"Failed to stop Key Interceptor.\nError: {}");
    }

    LRESULT CALLBACK WndProc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_CREATE: {
            AddNotifyIcon(hWnd);
            StartKeyInterceptor();
            return 0;
        }
        case WM_DESTROY: {
            StopKeyInterceptor();
            DeleteNotifyIcon(hWnd);
            PostQuitMessage(0);
            return 0;
        }
        case WM_NOTIFYICON: {
            if (lParam == WM_RBUTTONUP)
            {
                ShowNotifyIconMenu(hWnd);
                return 0;
            }
            break;
        }
        case WM_COMMAND: {
            if (wParam == ID_NOTIFYICONMENU_ABOUT)
            {
                ShowAboutDialog();
                return 0;
            }
            if (wParam == ID_NOTIFYICONMENU_EXIT)
            {
                DestroyWindow(hWnd);
                return 0;
            }
            break;
        }
        default:
            break;
        }

        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
} // namespace

namespace NotifyIcon
{
    void InitializeNotifyIcon()
    {
        const auto hInstance = GetModuleHandle(nullptr);

        WNDCLASS wndClass = {};
        wndClass.lpfnWndProc = WndProc;
        wndClass.hInstance = hInstance;
        wndClass.lpszClassName = L"NotifyIconClass";
        RegisterClass(&wndClass);

        CreateWindow(L"NotifyIconClass", L"ReControl", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                     CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
    }
} // namespace NotifyIcon
