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
            m_MenuBar.Initialize(m_WindowHandle);
            return 0;
        }
        // - Display --------------------------------
        case WM_DPICHANGED:
        {   // Sent when the effective dots per inch (dpi) for a window has changed.
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

        // - Interactions & Commands ----------------
        case WM_COMMAND: // Sent when the user invokes a command item from a menu
        {                //      when a control sends a notification message to its parent window
                         //      when an accelerator keystroke is translated
            auto menuID = static_cast<MenuBar::ID>(LOWORD(wParam));
            if (auto color = m_MenuBar.GetColorFromID(menuID)) // std::nullopt == false
            {
                m_BackgroundColor = color.value(); // *color also works
                InvalidateRect(m_WindowHandle, nullptr, FALSE);
            }
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
        // TODO: Use WM_MOUSEHOVER to display a tooltip/info bar of some sort displaying info on the ellipse
        // TODO: Use SetCapture, ReleaseCapture and WM_MOUSEMOVE to let user define the size/shape of an ellipse
        // TODO: Use WM_MOUSEWHEEL to adjust the scale of existing ellipses
        // TODO: Use DragDetect to allow user to move the existing ellipses
        // TODO: Use ClipCursor to prevent user from moving an ellipse offscreen
        // TODO: Detect keyboard input and allow user to type basic color names that affects new ellipses
        // TODO: Adjust the cursor image to the context of the program

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

        m_pRenderTarget->Clear(m_BackgroundColor); // Fills render target with solid color 
        
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
            const D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::SteelBlue);
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
    float dpi = static_cast<float>(GetDpiForWindow(m_WindowHandle)); // TODO: auto vs float... ?
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
        UpdateDpiScale(); // Always update scale first

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
        // TODO: Is this still needed?
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
        D2D1_ELLIPSE& ellipse = m_Ellipses.emplace_back(D2D1::Point2F(dipX, dipY), radius, radius);
        
        InvalidateRect(m_WindowHandle, nullptr, FALSE); // Sends WM_PAINT message
        
        LOG_PRINT("Added dot at ({}, {}). Total dots: {}",
            ellipse.point.x, ellipse.point.y, m_Ellipses.size());
    }
}

void MainWindow::OnRButtonDown(const int x, const int y)
{   // Overlapping ellipses are layered with the oldest on the bottom
    // A user would expect the topmost one to be removed first
    float dipX = PixelsToDips(x);
    float dipY = PixelsToDips(y);
    auto it = std::find_if(m_Ellipses.rbegin(), m_Ellipses.rend(),
        [dipX, dipY](const D2D1_ELLIPSE& ellipse)
        {
            float dx = ellipse.point.x - dipX;
            float dy = ellipse.point.y - dipY;
            float distanceSquared = (dx * dx) + (dy * dy);
            return distanceSquared <= (ellipse.radiusX * ellipse.radiusX);
        });

    if (it != m_Ellipses.rend())
    {   // Iterator invalidated after erase is called so print prior to the call
        if (m_Ellipses.size() > 1)
        {
            LOG_PRINT("Removed dot at ({}, {}). Total dots: {}",
                it->point.x, it->point.y, m_Ellipses.size() - 1);
        }
        else
        {
            LOG_PRINT("Cleared all dots.",
                it->point.x, it->point.y, m_Ellipses.size() - 1);
        }
        m_Ellipses.erase(std::prev(it.base()));
        InvalidateRect(m_WindowHandle, nullptr, FALSE);
    }
}
