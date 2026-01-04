#ifndef POPUPMODULE_H
#define POPUPMODULE_H

// ------ Project wide settings ---------------------
#include "config.h"

// ------ Homebrew ----------------------------------
#include "ResourceIDs.h"

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
    {   // TODO: Open up the CreateWindowExW's and comment the passed parameters
        m_ParentHandle = ownerHandle;
        m_ButtonHandle = CreateWindowExW(
            0, L"BUTTON", L"Show Text", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0,
            0, 0, 0, ownerHandle, (HMENU)ID::PopUpModule::ShowButton, nullptr, nullptr);
        m_TextboxHandle = CreateWindowExW(
            0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            0, 0, 0, 0, ownerHandle, (HMENU)ID::PopUpModule::Textbox, nullptr, nullptr);
        
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
