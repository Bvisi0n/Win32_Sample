// ------ Homebrew ----------------------------------
#include "Button.h"
#include "ControlActions.h"
#include "MainWindow.h"
#include "TextBox.h"
#include "UIConstants.h"

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
}
