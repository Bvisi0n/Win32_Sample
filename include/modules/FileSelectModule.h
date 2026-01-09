#ifndef FILESELECTMODULE_H
#define FILESELECTMODULE_H

// ------ Project wide settings ---------------------
#include "config.h"

// ------ Win32 and more ----------------------------
#include <windows.h>	// The core
#include <shobjidl.h>   // IFileOpenDialog (file open popup)
#include <wrl/client.h> // Microsoft::WRL::ComPtr (smartpointer)

// ------ STL ---------------------------------------
#include <filesystem>
#include <memory>

// ------ Homebrew ----------------------------------
#include "ResourceIDs.h"
#include "modules/UIModule.h"

class FileSelectModule : UIModule
{
public:
    FileSelectModule()  = default;
    ~FileSelectModule() = default;

    [[nodiscard]] int Initialize(HWND ownerHandle, float dpiScale, int yPosition) override
    {
        m_ParentHandle = ownerHandle;
        m_ButtonHandle = CreateWindowEx(
            0,                                                  // Optional window styles
            L"BUTTON",                                          // Predefined system class
            L"Select File...",                                  // Button text
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,              // Child window & button styles

            // Size and position (Set to 0, handled by UpdateLayout)
            0, 0, 0, 0,

            ownerHandle,                                        // Parent window handle
            ID::ToHandle(ID::FileSelectModule::SelectButton),   // Control ID
            GetModuleHandle(nullptr),                           // Instance handle
            nullptr                                             // Additional application data
        );

        m_LabelHandle = CreateWindowEx(
            0,                                              // Optional window styles
            L"STATIC",                                      // Predefined system class
            L"No file selected",                            // Button text
            WS_CHILD | WS_VISIBLE | SS_LEFT,                // Child window & button styles

            // Size and position (Set to 0, handled by UpdateLayout)
            0, 0, 0, 0,

            ownerHandle,                                    // Parent window handle
            ID::ToHandle(ID::FileSelectModule::InfoLabel),  // Control ID
            GetModuleHandle(nullptr),                       // Instance handle
            nullptr                                         // Additional application data
        );
        return UpdateLayout(dpiScale, yPosition); // Send back a new yPosition to feed to the next module
    }

    [[nodiscard]] int UpdateLayout(float dpiScale, int yPosition) const override
    {
        if (!m_ParentHandle || !m_ButtonHandle)
        {
            return yPosition; // Something's wrong -> send original yPosition to feed to the next module & do nothing
        }
        else
        {
            RECT clientRectangle;
            GetClientRect(m_ParentHandle, &clientRectangle);
            int parentWidth{ clientRectangle.right - clientRectangle.left };

            // Apply DPI to the fixed sizes
            int margin      { static_cast<int>(10.f * dpiScale) };
            int spacing     { static_cast<int>(10.f * dpiScale) };
            int height      { static_cast<int>(24.f * dpiScale) };
            int buttonWidth { static_cast<int>(120.f * dpiScale) };

            int availableWidth{ parentWidth - (2 * margin) - spacing};

            MoveWindow(m_ButtonHandle,  // Handle
                margin,                 // X position
                yPosition + margin,     // Y position
                buttonWidth,            // Width
                height,                 // Height
                TRUE);                  // Sends WM_PAINT to parent

            MoveWindow(m_LabelHandle,           // Handle
                margin + buttonWidth + spacing, // X position
                yPosition + margin,             // Y position
                availableWidth,                 // Width
                height * 2,                     // Height
                TRUE);                          // Sends WM_PAINT to parent

            return yPosition + margin + height * 2; // Send back a new yPosition to feed to the next module
        }
    }

    void OnButtonClicked()
    {
        using Microsoft::WRL::ComPtr;

        ComPtr<IFileOpenDialog> pFileOpen;
        HRESULT result = CoCreateInstance(  CLSID_FileOpenDialog,       // Class ID
                                            nullptr,                    // Used for Aggregation (NULL 99% of time)
                                            CLSCTX_ALL,                 // Tells Windows where the code for this object is allowed to run
                                            IID_PPV_ARGS(&pFileOpen));  // Extracts unique ID for the interface and casts your ComPtr address to void**
        
        if (SUCCEEDED(result))
        {
            result = pFileOpen->Show(m_ParentHandle); // Show the dialog

            if (SUCCEEDED(result))
            {   // Get the result (Shell Item)
                ComPtr<IShellItem> pItem;
                result = pFileOpen->GetResult(&pItem);

                if (SUCCEEDED(result))
                {   // Get the file path
                    PWSTR pszFilePath = nullptr; // Pointer String Zero-terminated FilePath
                    result = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    if (SUCCEEDED(result))
                    {   // R.1: Manage resources automatically using resource handles and RAII
                        std::unique_ptr<wchar_t, decltype(&CoTaskMemFree)> pathWrapper(pszFilePath, CoTaskMemFree);
                        try
                        {
                            std::filesystem::path filePath(pathWrapper.get());
                            auto fileSize = std::filesystem::file_size(filePath);

                            std::wstring infoText = std::format(L"File: {}\nSize: {} bytes",
                                                                filePath.filename().wstring(), fileSize);

                            SetWindowText(m_LabelHandle, infoText.c_str());
                        }
                        catch (const std::filesystem::filesystem_error& e)
                        {
                            SetWindowText(m_LabelHandle, L"Error: Could not read file size.");
                        }
                    }
                }
            }
        }
    }

private:
    HWND m_ButtonHandle = nullptr;
    HWND m_LabelHandle = nullptr;
};

#endif
