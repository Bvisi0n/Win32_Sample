// ------ Win32 and more ----------------------------
#include <d2d1.h>
#include <windows.h>

// ------ Homebrew ----------------------------------
#include "controls/TextBox.h"

TextBox::TextBox(UI::ControlID id, UI::Action action, unsigned int charLimit)
    : Control(id, action), m_CharLimit(charLimit) {}

void TextBox::Initialize(HWND parent, D2D1_RECT_F position)
{
    m_ControlHandle = CreateWindowEx(
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
    SendMessage(m_ControlHandle, EM_LIMITTEXT, m_CharLimit, 0);
    SendMessage(m_ControlHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
}

std::wstring TextBox::GetText() const
{
    int len = GetWindowTextLengthW(m_ControlHandle);
    std::wstring buffer;
    buffer.resize(len);
    GetWindowTextW(m_ControlHandle, buffer.data(), len + 1);
    return buffer;
}
