#ifndef BUTTON_H
#define BUTTON_H

// ------ Project wide settings ---------------------
#include "config.h"

// ------ Win32 and more ----------------------------
#include <d2d1.h>

// ------ STL ---------------------------------------
#include <string>
#include <string_view>

// ------ Homebrew ----------------------------------
#include "Control.h"
#include "UIConstants.h"

// ------ Forward Declarations ----------------------
struct HWND__;
    typedef struct HWND__* HWND;
struct HFONT__;
    typedef struct HFONT__* HFONT;

class Button : public Control
{
public:
    Button(UI::ControlID id, UI::Action action, std::wstring_view text);
        
    void Initialize(HWND parent, D2D1_RECT_F position) override;
    void UpdateLayout(float dpiScale, HFONT fontHandle) override;

    std::wstring_view GetText() const { return m_Text; };
    void SetText(std::wstring_view newText);

private:
    std::wstring m_Text;
};

#endif
