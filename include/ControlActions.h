#ifndef CONTROLACTIONS_H
#define CONTROLACTIONS_H

// ------ Project wide settings ---------------------
#include "Config.h"

// ------ Homebrew ----------------------------------
#include "UIConstants.h"

namespace UI
{
    // --- PopUpModule ------------------------------
    extern Action OnPopUpTextChanged;
    extern Action OnPopUpButtonClicked;

    // --- CursorModule -----------------------------
    extern Action OnArrowCursorClicked;
    extern Action OnHandCursorClicked;
    extern Action OnCrossCursorClicked;
}

#endif
