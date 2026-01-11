#ifndef CONTROLACTIONS_H
#define CONTROLACTIONS_H

// ------ Project wide settings ---------------------
#include "config.h"

// ------ Homebrew ----------------------------------
#include "Mainwindow.h"
#include "UIConstants.h"

namespace UI
{
    inline UI::Action OnSubmit = [](MainWindow* pWindow) {
        // Example: Access the Direct2D state from your project
        // pWindow->AddEllipse(100, 100); 
        std::println("Submit Triggered!");
        };

    inline UI::Action OnClear = [](MainWindow* pWindow) {
        // Logic to clear the m_Ellipses vector in your sample
        // pWindow->ClearAll();
        std::println("Clear Triggered!");
        };
}

#endif
