#ifndef CONTROL_H
#define CONTROL_H

// ------ Project wide settings ---------------------
#include "config.h"

// ------ Homebrew ----------------------------------
#include "ControlActions.h"
#include "UIConstants.h"

class Control {
public:
    Control(UI::ControlID id, UI::Action action)
        : m_Id(id), m_Action(action), m_Hwnd(nullptr) {
    }

    virtual ~Control() {
        if (m_Hwnd) DestroyWindow(m_Hwnd);
    }

    Control(const Control&) = delete;
    Control(Control&&) = delete;
    Control& operator=(const Control&) = delete;
    Control& operator=(Control&&) = delete;

    virtual void Initialize(HWND parent, D2D1_RECT_F rect) = 0;
    virtual void UpdateLayout(float dpiScale) = 0;

    void Execute(MainWindow* pWindow) { if (m_Action) m_Action(pWindow); }
    HWND GetHwnd() const { return m_Hwnd; }
    UI::ControlID GetId() const { return m_Id; }

protected:
    HWND m_Hwnd;
    UI::ControlID m_Id;
    UI::Action m_Action;
};

#endif
