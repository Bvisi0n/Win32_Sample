// ------ Homebrew ----------------------------------
#include "controls/Button.h"
#include "controls/ControlActions.h"
#include "controls/Label.h"
#include "controls/RadioButton.h"
#include "controls/TextBox.h"
#include "controls/UIConstants.h"
#include "windows/MainWindow.h"
#include "utilities/FileDialog.h"

namespace UI {
    Action OnPopUpTextChanged = [](MainWindow* pWin)
        {
            auto pBox = pWin->GetTextBox(UI::ControlID::PopUp_Textbox);
            auto pBtn = pWin->GetButton(UI::ControlID::PopUp_Button);

            if (pBox && pBtn)
            {
                bool hasText = GetWindowTextLengthW(pBox->GetWindowHandle()) > 0;
                EnableWindow(pBtn->GetWindowHandle(), hasText);
            }
        };

    Action OnPopUpButtonClicked = [](MainWindow* pWin)
        {
            auto pBox = pWin->GetTextBox(UI::ControlID::PopUp_Textbox);

            if (pBox)
            {
                std::wstring text = pBox->GetText();

                if (!text.empty())
                {
                    MessageBoxW(pWin->Window(), text.c_str(), L"You typed:", MB_OK);
                }
                else
                {
                    MessageBoxW(pWin->Window(), L"Nothing!\nError!!!", L"You typed:", MB_ICONWARNING);
                }
            }
        };

    Action OnArrowCursorClicked = [](MainWindow* pWin)
        {
            pWin->SetCursorType(UI::ControlID::Cursor_ArrowButton);
            // Force cursor repaint by telling OS the mouse "moved"
            SendMessage(pWin->Window(), WM_SETCURSOR, reinterpret_cast<WPARAM>(pWin->Window()), MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
        };

    Action OnHandCursorClicked = [](MainWindow* pWin)
        {
            pWin->SetCursorType(UI::ControlID::Cursor_HandButton);
            // Force cursor repaint by telling OS the mouse "moved"
            SendMessage(pWin->Window(), WM_SETCURSOR, reinterpret_cast<WPARAM>(pWin->Window()), MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
        };

    Action OnCrossCursorClicked = [](MainWindow* pWin)
        {
            pWin->SetCursorType(UI::ControlID::Cursor_CrossButton);
            // Force cursor repaint by telling OS the mouse "moved"
            SendMessage(pWin->Window(), WM_SETCURSOR, reinterpret_cast<WPARAM>(pWin->Window()), MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
        };

    Action OnDatePickerChanged = [](MainWindow* pWin)
        {
            LOG_PRINT("OnDatePickerChanged() called.");
        };

    Action OnFileSelectButtonClicked = [](MainWindow* pWin)
        {
            auto path = FileDialog::Open(pWin->Window(), { {L"All Files", L"*.*"} });

            // Find our Label to update it
            auto* pLabel = pWin->GetLabel(UI::ControlID::FileSelect_Label);
            if (!pLabel) return;

            if (path) {
                try {
                    auto fileSize = std::filesystem::file_size(*path);
                    std::wstring info = std::format(L"File: {}\nSize: {} bytes", path->filename().wstring(), fileSize);
                    pLabel->SetText(info);
                }
                catch (...) {
                    pLabel->SetText(L"Error reading file info.");
                }
            }
        };
}
