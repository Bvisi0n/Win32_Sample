#ifndef CONTROL_H
#define CONTROL_H

// ------ Project wide settings ---------------------
#include "Config.h"

// ------ Win32 and more ----------------------------
#include <d2dbasetypes.h>

// ------ Homebrew ----------------------------------
#include "controls/UIConstants.h"

// ------ Forward Declarations ----------------------
struct HWND__;
typedef struct HWND__* HWND;

struct HFONT__;
typedef struct HFONT__* HFONT;

class MainWindow;

class Control
{
public:
    Control(UI::ControlID id, UI::Action action);
    virtual ~Control();

    Control(const Control&) = delete;
    Control(Control&&) = delete;
    Control& operator=(const Control&) = delete;
    Control& operator=(Control&&) = delete;

    virtual void Initialize(HWND parent, D2D1_RECT_F position) = 0;

    void UpdateLayout(float dpiScale, HFONT fontHandle);
    void Execute(MainWindow* pWindow);

    HWND GetWindowHandle() const { return m_ControlHandle; }
    UI::ControlID GetId() const { return m_Id; }
    D2D1_RECT_F GetPosition() const { return m_PositionRect; }

    void SetPosition(float x, float y, float width, float height);

protected:
    UI::Action      m_Action;
    HWND            m_ControlHandle;
    D2D1_RECT_F     m_PositionRect;
    UI::ControlID   m_Id;
};

#endif
