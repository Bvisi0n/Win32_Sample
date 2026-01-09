#ifndef CURSORMODULE_H
#define CURSORMODULE_H

// ------ Project wide settings ---------------------
#include "config.h"

// ------ Win32 and more ----------------------------
#include <windows.h>	// The core

// ------ Homebrew ----------------------------------
#include "ResourceIDs.h"
#include "modules/UIModule.h"

class CursorModule : UIModule
{
public:
	CursorModule()  = default;
	~CursorModule() = default;

    [[nodiscard]] int Initialize(HWND ownerHandle, float dpiScale, int yPosition) override
	{
        m_ParentHandle      = ownerHandle;
        m_RadioArrowHandle  = CreateWindowEx(
            0,                                                      // Optional window styles
            L"BUTTON",                                              // Predefined system class
            L"Arrow",                                               // Button text
            WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,  // Child window & button styles

            // Size and position (Set to 0, handled by UpdateLayout)
            0, 0, 0, 0,

            ownerHandle,                                            // Parent window handle
            ID::ToHandle(ID::CursorModule::ArrowRadioButton),       // Control ID
            GetModuleHandle(nullptr),                               // Instance handle
            nullptr                                                 // Additional application data
        );

        m_RadioHandHandle   = CreateWindowEx(
            0,                                                  // Optional window styles
            L"BUTTON",                                          // Predefined system class
            L"Hand",                                            // Button text
            WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,         // Child window & button styles

            // Size and position (Set to 0, handled by UpdateLayout)
            0, 0, 0, 0,

            ownerHandle,                                        // Parent window handle
            ID::ToHandle(ID::CursorModule::HandRadioButton),    // Control ID
            GetModuleHandle(nullptr),                           // Instance handle
            nullptr                                             // Additional application data
        );

        m_RadioCrossHandle  = CreateWindowEx(
            0,                                                  // Optional window styles
            L"BUTTON",                                          // Predefined system class
            L"Crosshair",                                       // Button text
            WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,         // Child window & button styles

            // Size and position (Set to 0, handled by UpdateLayout)
            0, 0, 0, 0,

            ownerHandle,                                        // Parent window handle
            ID::ToHandle(ID::CursorModule::CrossRadioButton),   // Control ID
            GetModuleHandle(nullptr),                           // Instance handle
            nullptr                                             // Additional application data
        );

        SendMessageW(m_RadioArrowHandle, BM_SETCHECK, BST_CHECKED, 0); // Check Arrow as initial position
        return UpdateLayout(dpiScale, yPosition); // Send back a new yPosition to feed to the next module
	}

    [[nodiscard]] int UpdateLayout(float dpiScale, int yPosition) const override
    {
        if (!m_ParentHandle || !m_RadioArrowHandle || !m_RadioHandHandle || !m_RadioCrossHandle)
        {
            return yPosition; // Something's wrong -> send original yPosition to feed to the next module & do nothing
        }
        else
        {
            RECT clientRectangle;
            GetClientRect(m_ParentHandle, &clientRectangle);
            int parentWidth{ clientRectangle.right - clientRectangle.left };

            // Apply DPI to the fixed sizes
            int margin{ static_cast<int>(10.f * dpiScale) };
            int spacing{ static_cast<int>(10.f * dpiScale) };
            int height{ static_cast<int>(24.f * dpiScale) };

            int availableWidth{ parentWidth - (2 * margin) - (2 * spacing) };
            int SliceOfWidth{ availableWidth / 3 };

            MoveWindow(m_RadioArrowHandle,  // Handle
                margin,                     // X position
                yPosition + margin,         // Y position
                SliceOfWidth,               // Width
                height,                     // Height
                TRUE);                      // Sends WM_PAINT to parent

            MoveWindow(m_RadioHandHandle,           // Handle
                margin + SliceOfWidth + spacing,    // X position
                yPosition + margin,                 // Y position
                SliceOfWidth,                       // Width
                height,                             // Height
                TRUE);                              // Sends WM_PAINT to parent

            MoveWindow(m_RadioCrossHandle,              // Handle
                margin + 2 * (SliceOfWidth + spacing),  // X position
                yPosition + margin,                     // Y position
                SliceOfWidth,                           // Width
                height,                                 // Height
                TRUE);                                  // Sends WM_PAINT to parent

            return yPosition + margin + height; // Send back a new yPosition to feed to the next module
        }
    }

    [[nodiscard]] HCURSOR GetSelectedCursor() const
    {
        if (SendMessage(m_RadioArrowHandle, BM_GETCHECK, 0, 0) == BST_CHECKED)
        {
            return LoadCursor(nullptr, IDC_ARROW);
        }
        else if (SendMessage(m_RadioHandHandle, BM_GETCHECK, 0, 0) == BST_CHECKED)
        {
            return LoadCursor(nullptr, IDC_HAND);
        }
        else
        {
            return LoadCursor(nullptr, IDC_CROSS);
        }
        // When adding more, consider struct{HWND; LPCWSTR;} with a rfor loop
    }

private:
	HWND m_RadioArrowHandle = nullptr;
	HWND m_RadioHandHandle  = nullptr;
	HWND m_RadioCrossHandle = nullptr;
};

#endif
