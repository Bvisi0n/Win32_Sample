#ifndef DATEPICKER_H
#define DATEPICKER_H

// ------ Project wide settings ---------------------
#include "Config.h"

// ------ Win32 and more ----------------------------
#include <d2dbasetypes.h>

// ------ Homebrew ----------------------------------
#include "controls/Control.h"
#include "UIConstants.h"

// ------ Forward Declarations ----------------------
struct HWND__;
typedef struct HWND__* HWND;

struct HFONT__;
typedef struct HFONT__* HFONT;

class DatePicker : public Control
{
public:
    DatePicker(UI::ControlID id, UI::Action action);

    void Initialize(HWND parent, D2D1_RECT_F position) override;

    SYSTEMTIME GetDate() const;
    void SetDate(const SYSTEMTIME& st);
};

#endif