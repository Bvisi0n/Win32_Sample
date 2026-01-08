#ifndef POPUPMODULE_H
#define POPUPMODULE_H

// ------ Project wide settings ---------------------
#include "config.h"

// ------ Homebrew ----------------------------------
#include "ResourceIDs.h"
#include "UIModule.h"

// TODO 3b: Prevent the cursor from going into hiding when typing in the text field

class PopUpModule : UIModule
{
public:
	PopUpModule()           = default;
	~PopUpModule() override = default;

    [[nodiscard]] int Initialize(HWND ownerHandle, float dpiScale, int yPosition) override
    {
        m_ParentHandle = ownerHandle;
        m_ButtonHandle = CreateWindowEx(
            0,                                          // Optional window styles
            L"BUTTON",                                  // Predefined system class
            L"Show Text",                               // Button text
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,      // Child window & button styles

            // Size and position (Set to 0, handled by UpdateLayout)
            0, 0, 0, 0,

            ownerHandle,                                // Parent window handle
            ID::ToHandle(ID::PopUpModule::ShowButton),  // Control ID
            GetModuleHandle(nullptr),                   // Instance handle
            nullptr                                     // Additional application data
        );

        m_TextboxHandle = CreateWindowEx(
            0,                                                  // Optional window styles
            L"EDIT",                                            // Predefined system class
            L"",                                                // Prefilled text !!ShowButton starts disabled!!
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, // Child window & button styles

            // Size and position (Set to 0, handled by UpdateLayout)
            0, 0, 0, 0,
            
            ownerHandle,                                        // Parent window handle
            ID::ToHandle(ID::PopUpModule::Textbox),             // Control ID
            GetModuleHandle(nullptr),                           // Instance handle
            nullptr                                             // Additional application data
        );
        
        SendMessage(m_TextboxHandle, EM_LIMITTEXT, 500, 0); // Limit input to 500 characters

        EnableWindow(m_ButtonHandle, FALSE);
        return UpdateLayout(dpiScale, yPosition); // Send back a new yPosition to feed to the next module
    }

    [[nodiscard]] int UpdateLayout(float dpiScale, int yPosition) const override
    {
        if (!m_ParentHandle || !m_TextboxHandle || !m_ButtonHandle)
        {
            return yPosition; // Something's wrong -> send original yPosition to feed to the next module & do nothing
        }
        else
        {
            RECT clientRectangle;
            GetClientRect(m_ParentHandle, &clientRectangle);
            int parentWidth{ clientRectangle.right - clientRectangle.left };

            // Apply DPI to the fixed sizes
            int margin  { static_cast<int>(10.f * dpiScale) };
            int spacing { static_cast<int>(10.f * dpiScale) };
            int height  { static_cast<int>(24.f * dpiScale) };

            int availableWidth  { parentWidth - (2 * margin) - spacing };
            int halfWidth       { availableWidth / 2 };
        
            MoveWindow( m_TextboxHandle,                // Handle
                        margin,                         // X position
                        yPosition + margin,             // Y position
                        halfWidth,                      // Width
                        height,                         // Height
                        TRUE);                          // Sends WM_PAINT to parent

            MoveWindow( m_ButtonHandle,                 // Handle
                        margin + halfWidth + spacing,   // X position
                        yPosition + margin,             // Y position
                        halfWidth,                      // Width
                        height,                         // Height
                        TRUE);                          // Sends WM_PAINT to parent

            return yPosition + margin + height; // Send back a new yPosition to feed to the next module
        }
    }

    void OnTextChanged() const
    {
        EnableWindow(m_ButtonHandle, GetWindowTextLengthW(m_TextboxHandle) > 0);
    }

    void OnShowButtonClick() const
    {
        const int length{ GetWindowTextLengthW(m_TextboxHandle) };
        if (length > 0)
        {
            std::wstring buffer;
            buffer.resize(length);
            GetWindowTextW(m_TextboxHandle, buffer.data(), length + 1);
            MessageBoxW(m_ParentHandle, buffer.c_str(), L"You typed:", MB_OK);
        }
        else // This path should never happen
        {
            MessageBoxW(m_ParentHandle, L"You typed:", L"Nothing!\nError!!!", MB_ICONWARNING);
        }
    }

private:
	HWND m_ButtonHandle     = nullptr;
	HWND m_TextboxHandle    = nullptr;
};

#endif
