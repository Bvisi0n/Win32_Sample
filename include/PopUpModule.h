#ifndef POPUPMODULE_H
#define POPUPMODULE_H

// ------ Project wide settings ---------------------
#include "config.h"

// ------ Homebrew ----------------------------------
#include "ResourceIDs.h"

// TODO: Prevent the cursor from going into hiding when typing in the textfield

class PopUpModule
{
public:
	PopUpModule() = default;
	~PopUpModule() = default;
	PopUpModule(const PopUpModule&) = delete;
	PopUpModule(PopUpModule&&) = delete;
	PopUpModule& operator=(const PopUpModule&) = delete;
	PopUpModule& operator=(PopUpModule&&) = delete;

    void Initialize(HWND ownerHandle, float dpiScale)
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
        
        EnableWindow(m_ButtonHandle, FALSE);
        UpdateLayout(dpiScale);
    }

    void UpdateLayout(float dpiScale)
    {
        if (!m_ParentHandle || !m_TextboxHandle || !m_ButtonHandle)
        {
            return; // Something's wrong -> do nothing
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
                        margin,                         // Y position
                        halfWidth,                      // Width
                        height,                         // Height
                        TRUE);                          // Sends WM_PAINT to parent

            MoveWindow( m_ButtonHandle,                 // Handle
                        margin + halfWidth + spacing,   // X position
                        margin,                         // Y position
                        halfWidth,                      // Width
                        height,                         // Height
                        TRUE);                          // Sends WM_PAINT to parent
        }
    }

    void OnTextChanged()
    {
        EnableWindow(m_ButtonHandle, GetWindowTextLengthW(m_TextboxHandle) > 0);
    }

    void ExecuteAction()
    {
        if (GetWindowTextLengthW(m_TextboxHandle) > 0)
        {
            wchar_t buffer[256]{};
            GetWindowTextW(m_TextboxHandle, buffer, 256);
            MessageBoxW(m_ParentHandle, buffer, L"You typed:", MB_OK);
        }
    }

private:
    HWND m_ParentHandle = nullptr; // Keeps UpdateLayout logic abstracted away from the parent.
	HWND m_ButtonHandle = nullptr;
	HWND m_TextboxHandle = nullptr;
};

#endif
