#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project wide settings
#include "Config.h"

// STL
#include <algorithm>

// Homebrew
#include "BaseWindow.h"
#include "SafeRelease.h"

class MainWindow : public BaseWindow<MainWindow>
{
    void Resize(UINT flag, int width, int height);
public:
    MainWindow() { }

    PCWSTR  ClassName() const override;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};

inline void MainWindow::Resize(UINT flag, int width, int height) {}

inline PCWSTR MainWindow::ClassName() const
{
    return L"Circle Window Class";
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
    {
        if (MessageBox(m_hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
        {
            DestroyWindow(m_hwnd); // Deactivates window if user confirms, sends WM_DESTROY
        }
        return 0; // Message handled -> exit function
    }
    case WM_DESTROY:    // Sent after the window is deactivated, but before the destruction occurs
    {                   // In particular, before any child windows are destroyed
        if (GetWindow(m_hwnd, GW_OWNER) == NULL)
        {   // If hwnd doesn't have a parent, then it must be the main window
            PostQuitMessage(0); //Adds WM_QUIT to queue which will terminate message loop/program
        }
        return 0; // Message handled -> exit function
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps); // Fills the PAINTSTRUCT with repaint request info
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1)); // FillRect fills a rectangle by using the specified brush
        EndPaint(m_hwnd, &ps); //  Clears update region, signals to OS that window has completed painting itself
        return 0; // Message handled -> exit function
    }
    case WM_SIZE:
    {
        int width = LOWORD(lParam);     // Macro to get the low-order word
        int height = HIWORD(lParam);    // Macro to get the high-order word
        Resize((UINT)wParam, width, height); // Respond to the message
        return 0; // Message handled -> exit function
    }
    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam); // Performs the default action for the message
}

#endif
