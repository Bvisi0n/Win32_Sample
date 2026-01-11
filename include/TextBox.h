#ifndef TEXTBOX_H
#define TEXTBOX_H

// ------ Project wide settings ---------------------
#include "config.h"

// ------ Win32 and more ----------------------------
#include <windows.h>
#include <d2d1.h>

// ------ STL ---------------------------------------
#include <string_view>

// ------ Homebrew ----------------------------------
#include "Control.h"

class TextBox : public Control
{
public:
    using Control::Control; // Inherit constructor

    void Initialize(HWND parent, D2D1_RECT_F position) override
    {
        m_WindowHandle = CreateWindowEx(
            WS_EX_CLIENTEDGE,                                       // Optional window styles
            L"EDIT",                                                // Predefined system class
            L"",                                                    // Button text
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL,    // Child window & button styles

            // Size and position (Set to 0, handled by UpdateLayout)
            0, 0, 0, 0,

            parent,                                                 // Parent window handle
            reinterpret_cast<HMENU>(m_Id),                          // Control ID
            GetModuleHandle(nullptr),                               // Instance handle
            nullptr                                                 // Additional application data
        );

        m_PositionRect = position;
        SendMessage(m_WindowHandle, EM_LIMITTEXT, 500, 0);
        SendMessage(m_WindowHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
    }

    void UpdateLayout(float dpiScale) override
    {
        int x = static_cast<int>(m_PositionRect.left * dpiScale);
        int y = static_cast<int>(m_PositionRect.top * dpiScale);
        int w = static_cast<int>((m_PositionRect.right - m_PositionRect.left) * dpiScale);
        int h = static_cast<int>((m_PositionRect.bottom - m_PositionRect.top) * dpiScale);

        SetWindowPos(m_WindowHandle, nullptr, x, y, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
    }

    std::wstring GetText() const
    {
        int len = GetWindowTextLengthW(m_WindowHandle);
        std::wstring buffer;
        buffer.resize(len);
        GetWindowTextW(m_WindowHandle, buffer.data(), len + 1);
        return buffer;
    }

private:
    D2D1_RECT_F m_PositionRect;
};

#endif
