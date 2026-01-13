// ------ Win32 and more ----------------------------
#include <d2d1.h>
#include <windows.h>

// ------ Homebrew ----------------------------------
#include "controls/Control.h"
#include "MainWindow.h"

Control::Control(UI::ControlID id, UI::Action action)
    : m_Action(action),
    m_ControlHandle(nullptr),
    m_PositionRect{ 0.0f, 0.0f, 0.0f, 0.0f },
    m_Id(id) {}

Control::~Control()
{
    if (m_ControlHandle)
    {
        DestroyWindow(m_ControlHandle);
    }
}

void Control::UpdateLayout(float dpiScale, HFONT fontHandle)
{
    int x = static_cast<int>(m_PositionRect.left * dpiScale);
    int y = static_cast<int>(m_PositionRect.top * dpiScale);
    int w = static_cast<int>((m_PositionRect.right - m_PositionRect.left) * dpiScale);
    int h = static_cast<int>((m_PositionRect.bottom - m_PositionRect.top) * dpiScale);

    SetWindowPos(m_ControlHandle, nullptr, x, y, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
    SendMessage(m_ControlHandle, WM_SETFONT, (WPARAM)fontHandle, TRUE);
}

void Control::Execute(MainWindow* pWindow)
{
    if (m_Action) m_Action(pWindow);
}

void Control::SetPosition(float x, float y, float width, float height)
{
    m_PositionRect.left = x;
    m_PositionRect.right = x + width;
    m_PositionRect.top = y;
    m_PositionRect.bottom = y + height;
}