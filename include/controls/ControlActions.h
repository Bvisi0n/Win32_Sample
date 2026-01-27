#ifndef CONTROLACTIONS_H
#define CONTROLACTIONS_H

// ------ Project wide settings ---------------------
#include "Config.h"

// ------ Homebrew ----------------------------------
#include "controls/UIConstants.h"

namespace UI
{
    // --- PopUp ------------------------------------
    extern Action OnPopUpTextChanged;
    extern Action OnPopUpButtonClicked;

    // --- Cursor -----------------------------------
    extern Action OnArrowCursorClicked;
    extern Action OnHandCursorClicked;
    extern Action OnCrossCursorClicked;

    // --- DatePicker -------------------------------
    extern Action OnDatePickerChanged;

    // --- DatePicker -------------------------------
    extern Action OnFileSelectButtonClicked;
}

#endif
