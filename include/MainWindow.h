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
    ID2D1Factory*          m_pFactory = nullptr;
    ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;
    ID2D1SolidColorBrush*  m_pBrush = nullptr;
    D2D1_ELLIPSE           m_ellipse{};

    void    CalculateLayout();
    HRESULT CreateGraphicsResources();
    void    DiscardGraphicsResources();
    void    OnPaint();
    void    OnSize(UINT flag, int width, int height);

public:
    MainWindow() = default;
    ~MainWindow() override;

    PCWSTR  ClassName() const override;
    LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
};

inline void MainWindow::CalculateLayout()
{
    if (m_pRenderTarget != nullptr)
    {
        D2D1_SIZE_F size = m_pRenderTarget->GetSize();
        const float x = size.width / 2;
        const float y = size.height / 2;
        const float radius = std::min(x, y);
        m_ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
    }
}

inline HRESULT MainWindow::CreateGraphicsResources()
{
    HRESULT result = S_OK;
    if (m_pRenderTarget == nullptr)
    {
        RECT rectangle;
        GetClientRect(m_WindowHandle, &rectangle);

        D2D1_SIZE_U size = D2D1::SizeU(rectangle.right, rectangle.bottom);

        result = m_pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
                                              D2D1::HwndRenderTargetProperties(m_WindowHandle, size),
                                              &m_pRenderTarget);

        if (SUCCEEDED(result))
        {
            const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
            result = m_pRenderTarget->CreateSolidColorBrush(color, &m_pBrush);

            if (SUCCEEDED(result))
            {
                CalculateLayout();
            }
        }
    }
    return result;
}

inline void MainWindow::DiscardGraphicsResources()
{
    SafeRelease(&m_pBrush);
    SafeRelease(&m_pRenderTarget);
}

inline void MainWindow::OnPaint()
{
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(m_WindowHandle, &ps);

        m_pRenderTarget->BeginDraw();

        m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
        m_pRenderTarget->FillEllipse(m_ellipse, m_pBrush);

        hr = m_pRenderTarget->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources();
        }
        EndPaint(m_WindowHandle, &ps);
    }
}

inline void MainWindow::OnSize(UINT flag, int width, int height)
{
    if (m_pRenderTarget != nullptr)
    {
        RECT rc;
        GetClientRect(m_WindowHandle, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        m_pRenderTarget->Resize(size);
        CalculateLayout();
        InvalidateRect(m_WindowHandle, nullptr, FALSE);
    }
}

inline MainWindow::~MainWindow()
{   // LIFO, due to COM's internal reference counting
    DiscardGraphicsResources();
    SafeRelease(&m_pFactory);
}

inline PCWSTR MainWindow::ClassName() const
{
    return L"Main Window Class";
}

LRESULT MainWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CLOSE: // Sent as a signal that a window or an application should terminate
        {
            if (MessageBox(m_WindowHandle, L"This will exit the program.\nContinue?", L"Win32_Sample", MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(m_WindowHandle); // Deactivates window if user confirms, sends WM_DESTROY
            }
            return 0; // Message handled -> exit function
        }
        case WM_CREATE: // Sent when CreateWindow is called but before it returns
        {               // After the window is created, but before it becomes visible
            if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory)))
            {
                return -1; // Tell OS CreateWindow failed
            }
            return 0; // Message handled -> exit function
        }
        case WM_DESTROY: // Sent after the window is deactivated, but before the destruction occurs
        {                // In particular, before any child windows are destroyed
            if (GetWindow(m_WindowHandle, GW_OWNER) == nullptr)
            {   // If hwnd doesn't have a parent, then it must be the main window
                PostQuitMessage(0); //Adds WM_QUIT to queue which will terminate message loop/program
            }
            return 0; // Message handled -> exit function
        }
        case WM_PAINT: // Sent when OS or other app requests a (partial) repaint of the window
        {
            OnPaint();
            return 0; // Message handled -> exit function
        }
        case WM_SIZE: // Sent to a window after its size has changed
        {
            int width = LOWORD(lParam);          // Macro to get the low-order word
            int height = HIWORD(lParam);         // Macro to get the high-order word
            OnSize((UINT)wParam, width, height); // Respond to the message
            return 0; // Message handled -> exit function
        }
    }
    return DefWindowProc(m_WindowHandle, message, wParam, lParam); // Performs the default action for the message
}

#endif
