// ------ Win32 and more ----------------------------
#include <d2d1.h>
#include <windows.h>

// ------ Homebrew ----------------------------------
#include "controls/RadioButton.h"

RadioButton::RadioButton(UI::ControlID id, UI::Action action, std::wstring_view text, bool groupLeader)
    : Control(id, action), m_Text(text), m_GroupLeader(groupLeader) {}

void RadioButton::Initialize(HWND parent, D2D1_RECT_F position)
{
    if (m_GroupLeader)
    {
        m_ControlHandle = CreateWindowEx(
            0,                                                      // Optional window styles
            L"BUTTON",                                              // Predefined system class
            m_Text.c_str(),                                         // Button text
            WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,  // Child window & button styles

            // Size and position (Set to 0, handled by UpdateLayout)
            0, 0, 0, 0,

            parent,                                                 // Parent window handle
            reinterpret_cast<HMENU>(m_Id),                          // Control ID
            GetModuleHandle(nullptr),                               // Instance handle
            nullptr                                                 // Additional application data
        );
    }
    else
    {
        m_ControlHandle = CreateWindowEx(
            0,                                          // Optional window styles
            L"BUTTON",                                  // Predefined system class
            m_Text.c_str(),                             // Button text
            WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, // Child window & button styles

            // Size and position (Set to 0, handled by UpdateLayout)
            0, 0, 0, 0,

            parent,                                     // Parent window handle
            reinterpret_cast<HMENU>(m_Id),              // Control ID
            GetModuleHandle(nullptr),                   // Instance handle
            nullptr                                     // Additional application data
        );
    }

    m_PositionRect = position;
    SendMessage(m_ControlHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
}

void RadioButton::SetText(std::wstring_view newText)
{
    m_Text = newText;
    if (m_ControlHandle)
    {
        SetWindowTextW(m_ControlHandle, m_Text.c_str());
    }
}

bool RadioButton::IsChecked() const
{
    return SendMessage(m_ControlHandle, BM_GETCHECK, 0, 0) == BST_CHECKED;
}

void RadioButton::SetCheck()
{
    SendMessageW(m_ControlHandle, BM_SETCHECK, BST_CHECKED, 0);
}
