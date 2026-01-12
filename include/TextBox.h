#ifndef TEXTBOX_H
#define TEXTBOX_H

// ------ Project wide settings ---------------------
#include "config.h"

// ------ Win32 and more ----------------------------
#include <d2d1.h>

// ------ STL ---------------------------------------
#include <string>

// ------ Homebrew ----------------------------------
#include "Control.h"

// ------ Forward Declarations ----------------------
struct HWND__;
    typedef struct HWND__* HWND;
struct HFONT__;
    typedef struct HFONT__* HFONT;

class TextBox : public Control
{
public:
    using Control::Control;

    void Initialize(HWND parent, D2D1_RECT_F position) override;
    void UpdateLayout(float dpiScale, HFONT fontHandle) override;
    std::wstring GetText() const;
};

#endif
