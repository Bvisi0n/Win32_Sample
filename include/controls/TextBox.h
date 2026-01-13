#ifndef TEXTBOX_H
#define TEXTBOX_H

// ------ Project wide settings ---------------------
#include "Config.h"

// ------ Win32 and more ----------------------------
#include <d2dbasetypes.h>

// ------ STL ---------------------------------------
#include <string>

// ------ Homebrew ----------------------------------
#include "controls/Control.h"

// ------ Forward Declarations ----------------------
struct HWND__;
typedef struct HWND__* HWND;

struct HFONT__;
typedef struct HFONT__* HFONT;

class TextBox : public Control
{
public:
    TextBox(UI::ControlID id, UI::Action action, unsigned int charLimit);

    void Initialize(HWND parent, D2D1_RECT_F position) override;

    std::wstring GetText() const;

private:
    unsigned int m_CharLimit;
};

#endif
