#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H

// ------ Project wide settings ---------------------
#include "Config.h"

// ------ Win32 and more ----------------------------
#include <d2dbasetypes.h>

// ------ STL ---------------------------------------
#include <string>
#include <string_view>

// ------ Homebrew ----------------------------------
#include "controls/Control.h"
#include "controls/UIConstants.h"

// ------ Forward Declarations ----------------------
struct HWND__;
typedef struct HWND__* HWND;

struct HFONT__;
typedef struct HFONT__* HFONT;

class RadioButton : public Control
{
public:
    RadioButton(UI::ControlID id, UI::Action action, std::wstring_view text, bool groupLeader);

    void Initialize(HWND parent, D2D1_RECT_F position) override;

    std::wstring_view GetText() const { return m_Text; };
    void SetText(std::wstring_view newText);
    bool IsChecked() const;
    void SetCheck();

private:
    std::wstring m_Text;
    bool m_GroupLeader;
};

#endif