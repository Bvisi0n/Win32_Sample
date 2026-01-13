// ------ Win32 and more ----------------------------
#include <windows.h>
#include <CommCtrl.h>
#include <d2d1.h>

// ------ Homebrew ----------------------------------
#include "controls/DatePicker.h"

DatePicker::DatePicker(UI::ControlID id, UI::Action action)
    : Control(id, action) {}

void DatePicker::Initialize(HWND parent, D2D1_RECT_F position)
{
    m_ControlHandle = CreateWindowEx(
        0,                                                              // Optional window styles
        DATETIMEPICK_CLASS,                                             // Predefined system class
        L"",                                                            // Button text
        WS_BORDER | WS_CHILD | WS_VISIBLE | DTS_SHORTDATECENTURYFORMAT, // Child window & button styles

        // Size and position (Set to 0, handled by UpdateLayout)
        0, 0, 0, 0,

        parent,                                                         // Parent window handle
        reinterpret_cast<HMENU>(m_Id),                                  // Control ID
        GetModuleHandle(nullptr),                                       // Instance handle
        nullptr                                                         // Additional application data
    );

    SYSTEMTIME time;
    GetLocalTime(&time);

    SetDate(time);

    m_PositionRect = position;
    SendMessage(m_ControlHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
}

SYSTEMTIME DatePicker::GetDate() const
{
        SYSTEMTIME time;
        SendMessage(m_ControlHandle, DTM_GETSYSTEMTIME, 0, reinterpret_cast<LPARAM>(&time));
        return time;
}

void DatePicker::SetDate(const SYSTEMTIME& st)
{
    SendMessage(m_ControlHandle, DTM_SETSYSTEMTIME, GDT_VALID, reinterpret_cast<LPARAM>(&st));
}
