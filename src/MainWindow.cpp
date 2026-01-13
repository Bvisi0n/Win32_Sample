// ------ Win32 and more ----------------------------
#include <windows.h>
#include <windowsx.h>   // Required for GET_X_LPARAM and GET_Y_LPARAM

// ------ STL ---------------------------------------
#include <algorithm>    // Required for std::ranges::find_if
#include <ranges>       // Required for std::views::reverse

// ------ Homebrew ----------------------------------
#include "MainWindow.h"
#include "Control.h"
#include "Button.h"
#include "RadioButton.h"
#include "TextBox.h"
#include "DatePicker.h"
#include "ControlActions.h"
#include "FileDialog.h"
#include "FileService.h"
#include <Label.h>

// ----------------------------------------------
// ---- Special Member Functions ----------------
// ----------------------------------------------
MainWindow::MainWindow()
    :   BaseWindow(),
        m_DpiScale(1.0f),
        m_UIFontHandle(nullptr),
        m_BackgroundColor(D2D1::ColorF(D2D1::ColorF::AliceBlue)),
        m_EllipseSize(10.f),
        m_MenuBar(),
        m_CursorType(UI::ControlID::Cursor_ArrowButton) {}

// If declared default in the header it will attempt to define it in the header.
// Which doesn't pass std::unique_ptr's vibe check on the Control forward declaration.
MainWindow::~MainWindow() = default;

// ----------------------------------------------
// ---- Window Properties & Logic ---------------
// ----------------------------------------------
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
            UpdateDpiScale();

            if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_pFactory.GetAddressOf())))
            {
                return -1; // Tell OS CreateWindow failed
            }

            m_MenuBar.Initialize(m_WindowHandle);
            InitializeUI();
            return 0;
        }

        // - Display --------------------------------
        case WM_DPICHANGED:
        {   // Sent when the effective dots per inch (dpi) for a window has changed.
            UpdateDpiScale();
            UpdateControlLayouts();

            InvalidateRect(m_WindowHandle, nullptr, FALSE);
            return 0;
        }
        case WM_SIZE:
        {   // Sent to a window after its size has changed
            if (m_pRenderTarget != nullptr)
            {
                UpdateDpiScale(); // Always update scale first

                RECT rectangle;
                GetClientRect(m_WindowHandle, &rectangle);

                D2D1_SIZE_U size = D2D1::SizeU(rectangle.right, rectangle.bottom);
                m_pRenderTarget->Resize(size);

                InvalidateRect(m_WindowHandle, nullptr, FALSE); // Sends WM_PAINT message
            }
            UpdateControlLayouts();
            return 0;
        }
        case WM_ERASEBKGND:
        {
            return 1; // Removes flickering
        }
        case WM_PAINT:
        {   // Sent when OS or other app requests a (partial) repaint of the window
            OnPaint();
            return 0;
        }
        case WM_SETCURSOR:
        {
            if (LOWORD(lParam) == HTCLIENT)
            {
                HCURSOR cursorHandle{ nullptr };
                switch (m_CursorType)
                {
                case UI::ControlID::Cursor_ArrowButton:
                    cursorHandle = LoadCursor(nullptr, IDC_ARROW); break;
                case UI::ControlID::Cursor_HandButton:
                    cursorHandle = LoadCursor(nullptr, IDC_HAND);  break;
                case UI::ControlID::Cursor_CrossButton:
                    cursorHandle = LoadCursor(nullptr, IDC_CROSS); break;
                }
                SetCursor(cursorHandle);
                return TRUE;
            }
        }

        // - Interactions & Commands ----------------
        case WM_COMMAND: // Sent when the user invokes a command item from a menu
        {                //      when a control sends a notification message to its parent window
                         //      when an accelerator keystroke is translated
            const int id = LOWORD(wParam);
            const int code = HIWORD(wParam);

            if (HandleMenuBarCommands(id))
                return 0;

            auto it = m_Controls.find(static_cast<UI::ControlID>(id));
            if (it != m_Controls.end()) {
                // Only execute logic for clicks or text changes
                if (code == BN_CLICKED || code == EN_CHANGE) {
                    it->second->Execute(this);
                }
            }

            return 0;
        }

        // - Input ----------------------------------
        case WM_NOTIFY:
        {
            // Long Pointer Notification Message Header
            LPNMHDR lpnmhdr{ reinterpret_cast<LPNMHDR>(lParam) };
            
            if (lpnmhdr->code == DTN_DATETIMECHANGE)
            {
                auto it = m_Controls.find(static_cast<UI::ControlID>(lpnmhdr->idFrom));
                if (it != m_Controls.end())
                {
                    it->second->Execute(this);
                    SYSTEMTIME time;
                    time = dynamic_cast<DatePicker*>(it->second.get())->GetDate();
                    LOG_PRINT("{:02}/{:02}/{:04}", time.wDay, time.wMonth, time.wYear);
                }
            }
            return 0;
        }
        case WM_LBUTTONDOWN:
        {   // Left mouse button down in client area
            const int x{ GET_X_LPARAM(lParam) };
            const int y{ GET_Y_LPARAM(lParam) };
            OnLButtonDown(x, y);
            return 0;
        }
        case WM_RBUTTONDOWN:
        {   // Right mouse button down in client area
            const int x{ GET_X_LPARAM(lParam) };
            const int y{ GET_Y_LPARAM(lParam) };
            OnRButtonDown(x, y);
            return 0;
        }

        // - Destruction ----------------------------
        case WM_CLOSE:
        {   // Sent as a signal that a window or an application should terminate
            if (MessageBox(m_WindowHandle, L"This will exit the program.\nContinue?", L"Win32_Sample", MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(m_WindowHandle);
            }
            return 0;
        }
        case WM_DESTROY: // Sent after the window is deactivated, but before the destruction occurs
        {                // In particular, before any child windows are destroyed
            if (GetWindow(m_WindowHandle, GW_OWNER) == nullptr)
            {   // If hwnd doesn't have a parent, then it must be the main window
                PostQuitMessage(0);
            }
            return 0;
        }
    }
    return DefWindowProc(m_WindowHandle, message, wParam, lParam);
}

// ----------------------------------------------
// ---- Member Access ---------------------------
// ----------------------------------------------
Button* MainWindow::GetButton(UI::ControlID id)
{
    auto it = m_Controls.find(id);
    return (it != m_Controls.end()) ? static_cast<Button*>(it->second.get()) : nullptr;
}

Label* MainWindow::GetLabel(UI::ControlID id)
{
    auto it = m_Controls.find(id);
    return (it != m_Controls.end()) ? static_cast<Label*>(it->second.get()) : nullptr;
}

TextBox* MainWindow::GetTextBox(UI::ControlID id)
{
    auto it = m_Controls.find(id);
    return (it != m_Controls.end()) ? static_cast<TextBox*>(it->second.get()) : nullptr;
}

void MainWindow::SetCursorType(UI::ControlID cursorId)
{
    m_CursorType = cursorId;
}

// ----------------------------------------------
// ---- Initialization & Layout -----------------
// ----------------------------------------------
void MainWindow::InitializeUI()
{
    // -------- PopUpModule ---------------------
    auto pPopUpBox = std::make_unique<TextBox>(UI::ControlID::PopUp_Textbox, UI::OnPopUpTextChanged, 500);
    pPopUpBox->Initialize(m_WindowHandle, { 10, 10, 200, 34 });
    m_Controls[UI::ControlID::PopUp_Textbox] = std::move(pPopUpBox);

    auto pPopUpBtn = std::make_unique<Button>(UI::ControlID::PopUp_Button, UI::OnPopUpButtonClicked, L"Show Text");
    pPopUpBtn->Initialize(m_WindowHandle, { 210, 10, 310, 34 });
    m_Controls[UI::ControlID::PopUp_Button] = std::move(pPopUpBtn);

    // -------- CursorModule --------------------
    auto pCursorArrow = std::make_unique<RadioButton>(UI::ControlID::Cursor_ArrowButton, UI::OnArrowCursorClicked, L"Arrow", true);
    pCursorArrow->Initialize(m_WindowHandle, { 10, 50, 100, 74 });
    pCursorArrow->SetCheck();
    m_Controls[UI::ControlID::Cursor_ArrowButton] = std::move(pCursorArrow);

    auto pCursorHand = std::make_unique<RadioButton>(UI::ControlID::Cursor_HandButton, UI::OnHandCursorClicked, L"Hand", false);
    pCursorHand->Initialize(m_WindowHandle, { 110, 50, 200, 74 });
    m_Controls[UI::ControlID::Cursor_HandButton] = std::move(pCursorHand);

    auto pCursorCross = std::make_unique<RadioButton>(UI::ControlID::Cursor_CrossButton, UI::OnCrossCursorClicked, L"Cross", false);
    pCursorCross->Initialize(m_WindowHandle, { 210, 50, 300, 74 });
    m_Controls[UI::ControlID::Cursor_CrossButton] = std::move(pCursorCross);

    // -------- DatePicker ---------------------
    auto pDatePicker = std::make_unique<DatePicker>(UI::ControlID::DatePicker, UI::OnDatePickerChanged);
    pDatePicker->Initialize(m_WindowHandle, { 10, 90, 200, 125 });
    m_Controls[UI::ControlID::DatePicker] = std::move(pDatePicker);

    // -------- FileSelect ---------------------
    auto pFileSelectBtn = std::make_unique<Button>(UI::ControlID::FileSelect_Button, UI::OnFileSelectButtonClicked, L"Select File...");
    pFileSelectBtn->Initialize(m_WindowHandle, { 10, 150, 130, 174 });
    m_Controls[UI::ControlID::FileSelect_Button] = std::move(pFileSelectBtn);

    auto pFileSelectLab = std::make_unique<Label>(UI::ControlID::FileSelect_Label, nullptr, L"No file selected");
    pFileSelectLab->Initialize(m_WindowHandle, { 140, 150, 400, 198 });
    m_Controls[UI::ControlID::FileSelect_Label] = std::move(pFileSelectLab);

    SyncUIOrder();
    UpdateControlLayouts();

    UI::OnPopUpTextChanged(this); // Disable PopUp_Button
}

void MainWindow::SyncUIOrder()
{
    HWND hwndPrevious = HWND_TOP;

    for (auto const& [id, control] : m_Controls)
    {
        HWND hwndCurrent = control->GetWindowHandle();
        if (hwndCurrent)
        {
            SetWindowPos(hwndCurrent, hwndPrevious, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
            hwndPrevious = hwndCurrent;
        }
    }
}

void MainWindow::UpdateUIFont()
{
    if (m_UIFontHandle) DeleteObject(m_UIFontHandle);

    int fontSize = static_cast<int>(-12 * m_DpiScale);
    // TODO Refactor: clean this up
    m_UIFontHandle = CreateFontW(
        fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, VARIABLE_PITCH | FF_SWISS, L"Segoe UI"
    );
}

void MainWindow::UpdateControlLayouts()
{   // Use .SetPosition here to give Controls a dynamic position.
    UpdateUIFont();
    for (auto const& [id, control] : m_Controls)
    {
        control->UpdateLayout(m_DpiScale, m_UIFontHandle);
    }
}

// ----------------------------------------------
// ---- Rendering -------------------------------
// ----------------------------------------------
void MainWindow::OnPaint()
{
    HRESULT result = CreateGraphicsResources();
    if (SUCCEEDED(result))
    {
        PAINTSTRUCT paintStruct;
        BeginPaint(m_WindowHandle, &paintStruct);

        m_pRenderTarget->BeginDraw();
        m_pRenderTarget->Clear(m_BackgroundColor);

        for (auto& ellipse : m_Ellipses)
        {
            D2D1_ELLIPSE scaled = D2D1::Ellipse(
                D2D1::Point2F(ellipse.point.x * m_DpiScale, ellipse.point.y * m_DpiScale),
                ellipse.radiusX * m_DpiScale,
                ellipse.radiusY * m_DpiScale);

            m_pRenderTarget->FillEllipse(scaled, m_pBrush.Get());
        }

        // GPU might buffer the drawing commands
        // Flush() may be used in a way similar to std::endl for std::cout
        // EndDraw() also performs Flush(), think std::unitbuf
        // All return types are void, errors are signaled in return of EndDraw()
        result = m_pRenderTarget->EndDraw();
        if (FAILED(result) || result == D2DERR_RECREATE_TARGET)
        {   // D2DERR_RECREATE_TARGET is returned when the device is lost i.e. display resolution change, unplugged...
            m_pBrush.Reset();
            m_pRenderTarget.Reset();
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
        }
    }
    return result;
}

// ----------------------------------------------
// ---- Coordinate Systems ----------------------
// ----------------------------------------------
void MainWindow::UpdateDpiScale()
{
    float dpi{ static_cast<float>(GetDpiForWindow(m_WindowHandle)) };
    m_DpiScale = dpi / static_cast<float>(USER_DEFAULT_SCREEN_DPI);
    LOG_PRINT("Current DPI: {}, Scale: {}", dpi, m_DpiScale);
}

float MainWindow::PixelsToDips(int pixelValue) const
{
    return static_cast<float>(pixelValue) / m_DpiScale;
}

// ----------------------------------------------
// ---- Input Handling --------------------------
// ----------------------------------------------
bool MainWindow::HandleMenuBarCommands(const WORD id)
{
    auto menuID = static_cast<UI::ControlID>(id);
    if (auto color = m_MenuBar.GetColorFromID(menuID)) // std::nullopt == false
    {
        m_BackgroundColor = color.value(); // *color also works
        InvalidateRect(m_WindowHandle, nullptr, FALSE);
        return true;
    }

    auto makeFilters = [] -> std::vector<FileDialog::Filter>
    {
            return
            {
                { L"Binary Ellipses", L"*.belip" },
                { L"Text Ellipses", L"*.elip" },
                { L"All Files", L"*.*" }
            };
    };

    switch (menuID)
    {   //TODO New: Add background color to the data saved
        case UI::ControlID::Load:
        {
            auto path = FileDialog::Open(m_WindowHandle, makeFilters());
            if (path) FileService::Load(*path, m_Ellipses);
            InvalidateRect(m_WindowHandle, nullptr, FALSE); // Why is this a warning?
            return true;
        }
        case UI::ControlID::Save:
        {
            auto path = FileDialog::Save(m_WindowHandle, makeFilters(), L"belip");
            if (path) FileService::Save(*path, m_Ellipses);
            return true;
        }
    }
    return false;
}

void MainWindow::OnLButtonDown(const int x, const int y)
{
    if (m_pRenderTarget != nullptr)
    {
        const float dipX{ PixelsToDips(x) };
        const float dipY{ PixelsToDips(y) };
        const float radius{ 10.f };
        D2D1_ELLIPSE& ellipse = m_Ellipses.emplace_back(D2D1::Point2F(dipX, dipY), radius, radius);
        
        InvalidateRect(m_WindowHandle, nullptr, FALSE); // Sends WM_PAINT message
        
        LOG_PRINT("Added dot at ({}, {}). Total dots: {}",
            ellipse.point.x, ellipse.point.y, m_Ellipses.size());
    }
}

void MainWindow::OnRButtonDown(const int x, const int y)
{   
    // Overlapping ellipses are layered with the oldest on the bottom
    // A user would expect the topmost one to be removed first
    float dipX{ PixelsToDips(x) };
    float dipY{ PixelsToDips(y) };
    
    auto reversed_view = m_Ellipses | std::views::reverse;

    auto hitScan = [dipX, dipY](const D2D1_ELLIPSE& ellipse)
    {
            float dx{ ellipse.point.x - dipX };
            float dy{ ellipse.point.y - dipY };
            float distanceSquared{ (dx * dx) + (dy * dy) };
            return distanceSquared <= (ellipse.radiusX * ellipse.radiusX);
    };

    auto it = std::ranges::find_if(reversed_view, hitScan);
    if (it != reversed_view.end())
    {   
        // Log before erasing
        if (m_Ellipses.size() == 1)
        {
            LOG_PRINT("Cleared all dots.");
        }
        else
        {
            LOG_PRINT("Removed dot at ({}, {}). Total dots: {}",
                      it->point.x, it->point.y, m_Ellipses.size() - 1);
        }
        //  .erase() only accepts forward iterators
        m_Ellipses.erase(std::next(it).base());
        InvalidateRect(m_WindowHandle, nullptr, FALSE);
    }
}
