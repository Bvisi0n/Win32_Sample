// ------ Win32 and more ----------------------------
#include <d2d1.h>
#include <windows.h>

// ------ Homebrew ----------------------------------
#include "Button.h"

Button::Button(UI::ControlID id, UI::Action action, std::wstring_view text)
	: Control(id, action), m_Text(text) {}

void Button::Initialize(HWND parent, D2D1_RECT_F position)
{
    m_ControlHandle = CreateWindowEx(
        0,                                                      // Optional window styles
        L"BUTTON",                                              // Predefined system class
        m_Text.c_str(),                                         // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Child window & button styles

        // Size and position (Set to 0, handled by UpdateLayout)
        0, 0, 0, 0,

        parent,                                                 // Parent window handle
        reinterpret_cast<HMENU>(m_Id),                          // Control ID
        GetModuleHandle(nullptr),                               // Instance handle
        nullptr                                                 // Additional application data
    );

    m_PositionRect = position;
    SendMessage(m_ControlHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
}

void Button::SetText(std::wstring_view newText)
{
    m_Text = newText;
    if (m_ControlHandle)
    {
        SetWindowTextW(m_ControlHandle, m_Text.c_str());
    }
}
