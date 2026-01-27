// ------ Homebrew ----------------------------------
#include "controls/Button.h"
#include "controls/ControlActions.h"
#include "controls/Label.h"
#include "controls/RadioButton.h"
#include "controls/TextBox.h"
#include "controls/UIConstants.h"
#include "windows/SampleWindow.h"
#include "utilities/FileDialog.h"

namespace UI
{
    namespace Actions
    {
        namespace Sample
        {
            inline SampleWindow* Cast(IWindow* p)
            {
                return dynamic_cast<SampleWindow*>(p);
            }

            Action OnPopUpTextChanged = [](IWindow* pWin)
                {
                    auto pBox = Cast(pWin)->GetTextBox(UI::ControlID::PopUp_Textbox);
                    auto pBtn = Cast(pWin)->GetButton(UI::ControlID::PopUp_Button);

                    if (pBox && pBtn)
                    {
                        bool hasText = GetWindowTextLengthW(pBox->GetWindowHandle()) > 0;
                        EnableWindow(pBtn->GetWindowHandle(), hasText);
                    }
                };

            Action OnPopUpButtonClicked = [](IWindow* pWin)
                {
                    auto pBox = Cast(pWin)->GetTextBox(UI::ControlID::PopUp_Textbox);

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

            Action OnArrowCursorClicked = [](IWindow* pWin)
                {
                    Cast(pWin)->SetCursorType(UI::ControlID::Cursor_ArrowButton);
                    // Force cursor repaint by telling OS the mouse "moved"
                    SendMessage(pWin->Window(), WM_SETCURSOR, reinterpret_cast<WPARAM>(pWin->Window()), MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
                };

            Action OnHandCursorClicked = [](IWindow* pWin)
                {
                    Cast(pWin)->SetCursorType(UI::ControlID::Cursor_HandButton);
                    // Force cursor repaint by telling OS the mouse "moved"
                    SendMessage(pWin->Window(), WM_SETCURSOR, reinterpret_cast<WPARAM>(pWin->Window()), MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
                };

            Action OnCrossCursorClicked = [](IWindow* pWin)
                {
                    Cast(pWin)->SetCursorType(UI::ControlID::Cursor_CrossButton);
                    // Force cursor repaint by telling OS the mouse "moved"
                    SendMessage(pWin->Window(), WM_SETCURSOR, reinterpret_cast<WPARAM>(pWin->Window()), MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
                };

            Action OnDatePickerChanged = [](IWindow* pWin)
                {
                    LOG_PRINT("OnDatePickerChanged() called.");
                };

            Action OnFileSelectButtonClicked = [](IWindow* pWin)
                {
                    auto path = FileDialog::Open(pWin->Window(), { {L"All Files", L"*.*"} });

                    // Find our Label to update it
                    auto* pLabel = Cast(pWin)->GetLabel(UI::ControlID::FileSelect_Label);
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
    }
}
