// ------ STL ---------------------------------------
#include <algorithm>

// ------ Homebrew ----------------------------------
#include "MainWindow.h"
#include "SafeRelease.h"

// --------------------------------------------------
// ------------ Special Member Functions ------------
// --------------------------------------------------

MainWindow::~MainWindow()
{   // LIFO, due to COM's internal reference counting
    DiscardGraphicsResources();
    SafeRelease(&m_pFactory);
}

// -------------------------------------------------
// ----------- Window Properties & Logic -----------
// -------------------------------------------------

PCWSTR MainWindow::ClassName() const
{
    return L"Main Window Class";
}
LRESULT MainWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        // - Creation -------------------------------
        case WM_CREATE: // Sent when CreateWindow is called but before it returns
        {               // After the window is created, but before it becomes visible
            UpdateDpiScale(); // Set scale before creating resources
            if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory)))
            {
                return -1; // Tell OS CreateWindow failed
            }
            return 0;
        }
        // - Display --------------------------------
        case WM_DPICHANGED:
        {
            UpdateDpiScale();
            InvalidateRect(m_WindowHandle, nullptr, FALSE);
            return 0;
        }
        case WM_SIZE:
        {   // Sent to a window after its size has changed
            OnSize();
            return 0;
        }
        case WM_PAINT:
        {   // Sent when OS or other app requests a (partial) repaint of the window
            OnPaint();
            return 0;
        }

        // - Input ----------------------------------
        case WM_LBUTTONDOWN:
        {   // Left mouse button down in client area
            const int x = GET_X_LPARAM(lParam);
            const int y = GET_Y_LPARAM(lParam);
            OnLButtonDown(x, y);
            return 0;
        }
        case WM_RBUTTONDOWN:
        {   // Right mouse button down in client area
            const int x = GET_X_LPARAM(lParam);
            const int y = GET_Y_LPARAM(lParam);
            OnRButtonDown(x, y);
            return 0;
        }

        // - Destruction ----------------------------
        case WM_CLOSE:
        {   // Sent as a signal that a window or an application should terminate
            if (MessageBox(m_WindowHandle, L"This will exit the program.\nContinue?", L"Win32_Sample", MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(m_WindowHandle); // Sends WM_DESTROY
            }
            return 0; // Message handled -> exit function
        }
        case WM_DESTROY: // Sent after the window is deactivated, but before the destruction occurs
        {                // In particular, before any child windows are destroyed
            if (GetWindow(m_WindowHandle, GW_OWNER) == nullptr)
            {   // If hwnd doesn't have a parent, then it must be the main window
                PostQuitMessage(0); //Adds WM_QUIT to queue which will terminate message loop/program
            }
            return 0;
        }
    }
    return DefWindowProc(m_WindowHandle, message, wParam, lParam); // Performs the default action for the message
}

// -------------------------------------------------
// ------------------- Rendering -------------------
// -------------------------------------------------

void MainWindow::OnPaint()
{
    HRESULT result = CreateGraphicsResources();
    if (SUCCEEDED(result))
    {
        PAINTSTRUCT paintStruct;
        BeginPaint(m_WindowHandle, &paintStruct);

        m_pRenderTarget->BeginDraw();

        m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue)); // Fills render target with solid color 
        
        for (auto& ellipse : m_Ellipses)
        {
            m_pRenderTarget->FillEllipse(ellipse, m_pBrush); // Draws a filled ellipse with specified brush
        }

        // GPU might buffer the drawing commands
        // Flush() may be used in a way similar to std::endl for std::cout
        // EndDraw() also performs Flush(), think std::unitbuf
        // All return types are void, errors are signaled in return of EndDraw()
        result = m_pRenderTarget->EndDraw();
        if (FAILED(result) || result == D2DERR_RECREATE_TARGET)
        {   // D2DERR_RECREATE_TARGET is returned when the device is lost i.e. display resolution change, unplugged...
            DiscardGraphicsResources();
        }
        EndPaint(m_WindowHandle, &paintStruct);
    }
}

HRESULT MainWindow::CreateGraphicsResources()
{   // Creating a resource is expensive so prevent doing it every message.
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
            const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0); // RGB: Yellow
            result = m_pRenderTarget->CreateSolidColorBrush(color, &m_pBrush);

            if (SUCCEEDED(result))
            {
                CalculateLayout();
            }
        }
    }
    return result;
}

void MainWindow::DiscardGraphicsResources()
{   // LIFO, due to COM's internal reference counting
    SafeRelease(&m_pBrush);
    SafeRelease(&m_pRenderTarget);
}

// --------------------------------------------------
// --------------- Coordinate Systems ---------------
// --------------------------------------------------

void MainWindow::UpdateDpiScale()
{
    float dpi = static_cast<float>(GetDpiForWindow(m_WindowHandle));
    m_DpiScale = dpi / static_cast<float>(USER_DEFAULT_SCREEN_DPI);
}

float MainWindow::PixelsToDips(int pixelValue) const
{
    return static_cast<float>(pixelValue) / m_DpiScale;
}

// --------------------------------------------------
// --------------------- Layout ---------------------
// --------------------------------------------------

void MainWindow::OnSize()
{
    if (m_pRenderTarget != nullptr)
    {
        RECT rectangle;
        GetClientRect(m_WindowHandle, &rectangle);

        D2D1_SIZE_U size = D2D1::SizeU(rectangle.right, rectangle.bottom);

        m_pRenderTarget->Resize(size);
        CalculateLayout();
        InvalidateRect(m_WindowHandle, nullptr, FALSE); // Sends WM_PAINT message
    }
}

void MainWindow::CalculateLayout()
{
    if (m_pRenderTarget != nullptr)
    {
        //TODO: I want my circles to remain in relative position and scale to the window if it resizes.
        //D2D1_SIZE_F size = m_pRenderTarget->GetSize();
        //const float x = size.width / 2;
        //const float y = size.height / 2;
        //const float radius = std::min(x, y);
        //m_ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
    }
}

// -------------------------------------------------
// --------------------- Input ---------------------
// -------------------------------------------------

void MainWindow::OnLButtonDown(const int x, const int y)
{
    if (m_pRenderTarget != nullptr)
    {
        const float dipX = PixelsToDips(x);
        const float dipY = PixelsToDips(y);
        const float radius = 10.f;
        m_Ellipses.emplace_back(D2D1::Point2F(dipX, dipY), radius, radius);
        InvalidateRect(m_WindowHandle, nullptr, FALSE); // Sends WM_PAINT message
    }
}

void MainWindow::OnRButtonDown(const int x, const int y)
{
    float dipX = PixelsToDips(x);
    float dipY = PixelsToDips(y);
    auto it = std::find_if(m_Ellipses.begin(), m_Ellipses.end(),
        [dipX, dipY](const D2D1_ELLIPSE& ellipse)
        {
            float dx = ellipse.point.x - dipX;
            float dy = ellipse.point.y - dipY;
            float distanceSquared = (dx * dx) + (dy * dy);
            return distanceSquared <= (ellipse.radiusX * ellipse.radiusX);
        });

    if (it != m_Ellipses.end())
    {
        m_Ellipses.erase(it);
        InvalidateRect(m_WindowHandle, nullptr, FALSE);
    }
}
