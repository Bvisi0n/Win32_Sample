// ------ Win32 and more ----------------------------
#include <d2d1.h>
#include <windows.h>

// ------ Homebrew ----------------------------------
#include "Control.h"
#include "MainWindow.h"

Control::Control(UI::ControlID id, UI::Action action)
    : m_Action(action),
    m_WindowHandle(nullptr),
    m_PositionRect{ 0.0f, 0.0f, 0.0f, 0.0f },
    m_Id(id) {}

Control::~Control()
{
    if (m_WindowHandle)
    {
        DestroyWindow(m_WindowHandle);
    }
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